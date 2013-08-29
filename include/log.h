/*
Copyright 2013 Zynga Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
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
