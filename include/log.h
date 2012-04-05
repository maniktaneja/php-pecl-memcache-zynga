#ifndef _LOG_H_
#define _LOG_H_

#define ERRORLOG_STDERR        1
#define ERRORLOG_FILE          2
#define ERRORLOG_SYSLOG        3

#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stderr,"%s %d "fmt"\n",__FILE__,__LINE__, ##__VA_ARGS__);
#else
#define LOG(fmt, ...)
#endif

#define LOG_IDENT_PECL   "pecl-memcache"
enum syslogType {PECL_TYPE=LOG_LOCAL4|LOG_DEBUG, APACHE_TYPE=LOG_LOCAL5|LOG_DEBUG};

class logOutPut {
public:
   virtual int open(const char *logFile) = 0; 
   virtual void close() = 0;
   virtual int write(const char *fmt, ...) = 0;
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
    syslogOut():type(PECL_TYPE) {
    };
    
    syslogOut(syslogType t):type(t) {
    };

   int open(const char *p); 
   void close();
   int write(const char *fmt, ...);
private:
   syslogType type;
};

#endif 
