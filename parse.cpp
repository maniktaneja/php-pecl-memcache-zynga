#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
#include <stack>
#include <assert.h>
#include "parser.h"

opr *oprFactory(oprType op) {
    switch (op) {
        case GT:
            return new gt();
        case LT:
            return new lt();
        case AND:
            return new ad();
        case OR:
            return new oor();
        case BrcsStr:
            return new bs();
        case BrcsStp:
            return new be();
        case SEQ:
            return new s_EQ();
        case EQ:
            return new eq();
        case CTN:
            return new ctn();
        case PM:
            return new pm();
    }
}

#define __TN__(_type, _field, _name, _opndType)     \
    if (*itr == _name) {                            \
        tk.offset = offsetof(mc_logger_t, _field);  \
        tk.ty = OPRND;                              \
        tk.opndType = _opndType;                    \
    } else                                          
               
token exprParser :: getNextToken() {
    token tk;
    tk.ty = NA;   
    if (itr == strs.end()) {
        tk.ty = END;
        return tk;
    }
        
    RULE_FIELDS

#undef __TN__
    if (*itr == ">") {
        tk.ty = OPRTR;
        tk.op = GT;
    }
    else if (*itr == "<") {
        tk.ty = OPRTR;
        tk.op = LT;
    }
    else if (*itr == "&") {
        tk.ty = OPRTR;
        tk.op = AND;
    }
    else if (*itr == "|") {
        tk.ty = OPRTR;
        tk.op = OR;
    }
    else if (*itr == "(") {
        tk.ty = OPRTR;
        tk.op = BrcsStr;
    }
    else if (*itr == ")") {
        tk.ty = OPRTR;
        tk.op = BrcsStp;
    }
    else if (*itr == "=") {
        tk.ty = OPRTR;
        tk.op = EQ;
    }
    else if (*itr == "eq" || *itr == "EQ") {
        tk.ty = OPRTR;
        tk.op = SEQ;
    }
    else if (*itr == "prefix" || *itr == "PREFIX") {
        tk.ty = OPRTR;
        tk.op = PM;
    }
    else if (*itr == "contains" || *itr == "CONTAINS") {
        tk.ty = OPRTR;
        tk.op = CTN;
    }
    else {
        tk.ty = VALUE;
        LOG("value = %s", (*itr).c_str()); 
    }
    
    tk.str = *itr;
    itr++;
    return tk;
}



void exprParser :: printStrs() {
    std::vector<std::string>::iterator i = itr;
    while (i != strs.end()) {
        LOG("%s", (*i).c_str());
        i++;
    }
}

bool exprParser::checkInvalidOpr(genNode *oprNode, genNode *leftNode, genNode *rightNode) {
    if (oprNode->op && oprNode->op->strType) {
        assert(leftNode->left  == NULL);
        assert(leftNode->right  == NULL);
        if (((leaf *)leftNode)->getOpndType() != STRING) {
            LOG("Invalid oprator");
            return false;
        }
        if (((leaf *)leftNode)->getType() != OPRND) {
            return false;
        }
    }
    else if (((leaf *)leftNode)->left == NULL) { 
        if (((leaf *)leftNode)->getType() != OPRND) {    
            LOG("Invalid oprand");
            return false;
        }
        else if (oprNode->op->opt == OR || oprNode->op->opt == AND) {
            LOG("Invalid oprator");
            return false;
        }
    }

    return !((!!leftNode->left) ^ (!!rightNode->left));
}

bool exprParser :: buildTree() {
    std::stack<genNode *> oprStk;
    std::stack<genNode *> opndStk;
    bool success = true;

    printStrs();

    while (1) {
        token tk;
        if (success) {
            tk = getNextToken();
        }
        else {
            tk.ty = ERROR;
        }
        LOG("%s %d", tk.str.c_str(), tk.ty);
        switch (tk.ty)
        {
            case OPRND:
                opndStk.push(new leaf(tk.offset, tk.opndType, tk.ty));
                break;
            case VALUE:
                opndStk.push(new leaf(tk.str, tk.ty));
                break;
            case OPRTR:
                if (tk.op <= BrcsStr) {
                    if (oprStk.empty() || oprStk.top()->op->opt <= tk.op ||
                            oprStk.top()->op->opt == BrcsStr) {
                        oprStk.push(new nonLeaf(oprFactory(tk.op)));
                    } 
                    else {
                        if (opndStk.size() > 1) {
                            genNode *n = oprStk.top();
                            oprStk.pop();
                            n->right = opndStk.top();
                            opndStk.pop();     
                            n->left = opndStk.top();     
                            opndStk.pop();     
                            opndStk.push(n);
                            oprStk.push(new nonLeaf(oprFactory(tk.op)));
                            success = checkInvalidOpr(n, n->left, n->right);
                        } else {
                            success = false;
                        }
                    } 
                }
                else {
                    while (!oprStk.empty() && oprStk.top()->op->opt != BrcsStr && 
                        opndStk.size() > 1 && success) {
                        genNode *n = oprStk.top();
                        oprStk.pop();
                        n->right = opndStk.top();   
                        opndStk.pop();  
                        n->left = opndStk.top();   
                        opndStk.pop();  
                        opndStk.push(n);
                        success = checkInvalidOpr(n, n->left, n->right);
                    }
                    if (oprStk.empty() || oprStk.top()->op->opt != BrcsStr) {
                        success = false;
                    }
                    else {
                        genNode *n = oprStk.top();
                        oprStk.pop();
                        delete n;
                    }
                }
                break;
            case END:
                while (!oprStk.empty() && success) {
                    if (opndStk.size() > 1) {
                        genNode *n = oprStk.top();
                        oprStk.pop();
                        n->right = opndStk.top();   
                        opndStk.pop();  
                        n->left = opndStk.top();   
                        opndStk.pop(); 
                        opndStk.push(n);
                        success = checkInvalidOpr(n, n->left, n->right);
                        continue;
                    }
                    success = false;
                    break;
                }
                if (success) {
                    if (opndStk.size() == 1 && 
                        opndStk.top()->op) {
                        root = opndStk.top();
                        return true;
                    }
                    success = false;
                }
                break;
            case ERROR:
                genNode *p;
                LOG("In the Error");
                while (!opndStk.empty()) {
                    p = opndStk.top(); 
                    opndStk.pop();
                    destroyTree(p);
                }
                while (!oprStk.empty()) {
                    p = oprStk.top();
                    delete p;
                    oprStk.pop();
                }
                return false;
            default:
                LOG("Invalid token");
        }
    }
    return true;
}

int exprParser :: evaluateTree(data *d, genNode *ptr) {
    int result1, result2; 
   if (ptr == NULL) {
        return false;
    }
    if (ptr->left->left == NULL) {
        return ptr->evaluate(d, (leaf *)ptr->left, (leaf *)ptr->right);
    }
    result1 = evaluateTree(d, ptr->left);
    if (ptr->shortcircuit(result1)) {
        LOG("short circuiting");
        return result1;
    }
    result2 = evaluateTree(d, ptr->right);
    return ptr->evaluate(result1, result2);
}

void exprParser::destroyTree(genNode *ptr) {
    if (ptr) {
        destroyTree(ptr->left);
        destroyTree(ptr->right);
        delete ptr;
    }
}

