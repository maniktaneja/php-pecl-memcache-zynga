#ifndef __EXPR_PARSER_
#define __EXPR_PARSER_
#include "logger.h"
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<iterator>
#include<stack>
#include<map>
using namespace std;

#define ERRORLOG_STDERR        1
#define ERRORLOG_FILE          2
#define ERRORLOG_SYSLOG        3

enum tokenType {OPRTR, OPRND, VALUE, END, NA, ERROR};
enum oprType {OR, AND, PM, EQ, NEQ, CTN, GT, LT, SEQ, BrcsStr, BrcsStp};

#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr,"%s %d "fmt"\n",__FILE__,__LINE__, ##__VA_ARGS__);
#else
#define LOG(fmt, ...)
#endif

class token {
public:
    tokenType ty;
    oprType op;
    string str;
    off_t offset;
    field_type opndType;
};

class data : public mc_logger_t
{
};

class opr {
public:
    virtual bool eval(int a, int b) {} 
    virtual bool eval(char *a, string b) {}; 
    oprType opt;
    bool strType;     
};

class eq : public opr {
public:
    eq() {
        opt = EQ;
        strType = false;    
    }
    bool eval(int a, int b) {
        LOG("inside eq");
        return a == b;
    }
};

class ctn : public opr {
public:
    ctn() {
        opt = CTN;
        strType = true ;    
    }
    bool eval(char *a, string b) {
        string ab(a);
        return (ab.find(b) != string::npos);
    }
};

class gt : public opr {
public:
    gt() {
        opt = GT;
        strType = false;    
    }
    bool eval(int a, int b) {
        return a > b;
    }
};

class lt : public opr {
public:
    lt() {
        opt = LT;
        strType = false;    
    }
    bool eval(int a, int b) {
        return a < b;
    }
};

class ad : public opr {
public:
    ad() {
        opt = AND;
        strType = false;    
    }
    bool eval(int a, int b) {
        return a & b;
    }
};

class bs : public opr {
public:
    bs() {
        opt = BrcsStr;
        strType = false;    
    }
    bool eval(int a, int b) {
        return false;
    }
};

class be : public opr {
public:
    be() {
        opt = BrcsStp;
        strType = false;    
    }
    bool eval(int a, int b) {
        return false;
    }
};

class oor : public opr {
public:
    oor() {
        opt = OR;
        strType = false;    
    }
    bool eval(int a, int b) {
        return a | b;
    }
};

class s_EQ : public opr {
public:
    s_EQ() {
        opt = SEQ;
        strType = true;    
    }
    bool eval(char *a, string b) {
        if (!a) {
            return false;
        }
        return !strcmp(a, b.c_str());
    }
};

class pm : public opr {
public:
    pm() {
        opt = PM;
        strType = true;    
    }
    bool eval(char *a, string b) {
        if (!a) {
            return false;
        }
        return !strncmp(a, b.c_str(), b.size());
    }
};


class leaf;
class genNode{
public:
    genNode() {
        left = right = NULL;
        op = NULL;
    } 
    ~genNode() {
        delete op;
    }  
    virtual int evaluate(int a, int b) {}
    virtual int evaluate(data *d, leaf *left, leaf *right) {}
    virtual int shortcircuit(bool result) {}
    genNode *left;
    genNode *right;
    opr * op;    
    string str;
};

class leaf : public genNode {
public:
    leaf(string &p, tokenType &tp) {
        str.assign(p);
        val = atoi(p.c_str());    
        leafType = tp;
        opndType = INVAL; 
    }

    leaf(off_t t, field_type ot, tokenType tp) {
        off = t;
        leafType = tp;
        opndType = ot;
    }
    int getInt() {
        return val;
    }  
    string & getString() {
        return str;
    }
    off_t getOffset() {
        return off;
    }
    field_type getOpndType() {
        return opndType;
    }
    tokenType getType() {
        return leafType;
    }
private:
    field_type opndType; 
    int val;
    off_t off;
protected:
    tokenType leafType;
}; 

#define GET_VALUE(d,offset,type) *((type *)((char *)d + offset))

class nonLeaf : public genNode {
public:
   nonLeaf(opr *o) {
        op = o;
    }

    int evaluate(int left, int right) {
        return op->eval(left, right);
    } 
 
    int evaluate(data *d, leaf *left, leaf *right) {
        if (op->strType) {
            return op->eval(GET_VALUE(d, left->getOffset(), char *), right->getString());
        }
        else {
            return op->eval(GET_VALUE(d, left->getOffset(), int), right->getInt());
        }
    }
    
    int shortcircuit(bool result) {
        return result ? op->opt == OR : op->opt == AND; 
    }
}; 

class logOutPut {
public:
   virtual int open(const char *logFile) = 0; 
   virtual void close() = 0;
   virtual int write(const char *fmt, ...) = 0;
};

class exprParser {
public:
    exprParser(string &st) : str(st), root(NULL), out(NULL){
        istringstream iss(st.c_str());
        copy(istream_iterator<string>(iss),
            istream_iterator<string>(),
            back_inserter<vector<string> >(strs));
        itr = strs.begin();
    }
    bool buildTree();
    int evaluateTree(data *d, genNode *root);
    void destroyTree(genNode *p);
    token getNextToken();
    void printStrs();
    bool checkInvalidOpr(genNode* a, genNode* b, genNode *c);
    genNode *getRoot() {
        return root;
    }
    void setLogOutPut(logOutPut *p) {
        out = p;    
    }
    logOutPut *getLogOutPut() {
        return out;
    }
    ~exprParser() {
        if (out) {
            out->close();
            delete out;
        }
        if (root) {
            destroyTree(root);
        }
    }
private:
    genNode *root;
    string str;
    vector<string> strs;
    vector<string>::iterator itr;
    logOutPut *out; 
};

class logger {
public:
    static logger *instance() {
        static logger *ins;
        if (ins == NULL) {
            ins = new logger();
        }
        return ins;
    }
    bool insertExprTree(string mc, exprParser *p) {
        std::map<string, exprParser *> :: iterator it; 
        if ((it = exprMap.find(mc)) != exprMap.end()) {
            LOG("duplicate mc value %s, so returning", mc.c_str());
            return false;
        }
        exprMap[mc] = p;
        return true;
    }
    exprParser *getExprTree(string mc) {
        std::map<string, exprParser *> :: iterator it; 
        if ((it = exprMap.find(mc)) == exprMap.end()) {
            return NULL;
        }
        return it->second;
    }
    void flushConfig() {
        std::map<string, exprParser *> :: iterator itr;
        itr = exprMap.begin(); 
        while (itr != exprMap.end()) {
            delete itr->second;
            exprMap.erase(itr++);
        }
    }
private:
    logger() {
    }
    std::map<string, exprParser *> exprMap;
};

class fileOut : public logOutPut {
public:
   int open(const char *logFile); 
   void close();
   int write(const char *fmt, ...);
    fileOut():logFd(-1),logMode(ERRORLOG_FILE) {
    }
private:
    int logFd;   
    int logMode;
};

class syslogOut : public logOutPut {
public:
   int open(const char *p); 
   void close();
   int write(const char *fmt, ...);
};
#endif
