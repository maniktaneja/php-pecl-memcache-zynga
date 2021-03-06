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
#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <sys/time.h>
#include <syslog.h>
#include <sstream>
#include <stack>
#include<map>
#include<assert.h>
#include "log.h"

enum field_type {INVAL, STRING, NUMBER};
enum cmdType {OTHERS = 0, GET, SET};
class RequestLogger;
class logOutPut;
struct mc_logger;
struct LoggerData;
typedef struct timeval timeStruct;
typedef std::map<std::string, mc_logger *> keyLoggerMap_t;
extern std::stack<LoggerData *> loggerStack;
extern int maxLoggerSize;

class Timer {
protected:
    inline void recordTime(timeStruct *t) {
        gettimeofday(t, 0);
    }

    unsigned int diffTime(timeStruct &startTime, timeStruct &endTime) {
        return (endTime.tv_sec - startTime.tv_sec) * 1000 * 1000 +
            (endTime.tv_usec - startTime.tv_usec);
    }
};

#define RULE_FIELDS                                         \
        __TN__(char *, host, "host", STRING)                \
        __TN__(char *, key, "key", STRING)                  \
        __TN__(char *, command, "command", STRING)          \
        __TN__(int,    flags, "flags", NUMBER)              \
        __TN__(int,   res_code, "statuscode", NUMBER)       \
        __TN__(unsigned int,  res_time, "restime", NUMBER)  \
        __TN__(unsigned int,  res_len, "reslen", NUMBER)    \
        __TN__(unsigned int,  serial_time, "sertime", NUMBER) \
        __TN__(unsigned int,  expiry, "expiry", NUMBER)

#define __TN__(a,b,c,d) a b;
typedef struct mc_logger : public Timer {
    RULE_FIELDS

    void setHost(char *ln) {
        host = ln;
    }

    void setKey(const char *ln) {
        key = (char *)ln;
    }

    char *getKey() {
        return key;
    }

    void setCmd(char *ln) {
        command = ln;
    }

    void setFlags(int ln) {
        flags = ln;
    }

    void setCode(int ln) {
        res_code = ln;
    }

    int getCode() {
        return res_code;
    }

    void setResTime(unsigned int ln) {
        res_time = ln;
    }

    void setResLen(int ln) {
        res_len = ln;
    }

    void setLogName(char *ln) {
        log_name = ln;
    }

    void setCas(unsigned long c) {
        cas = c;
    }

    void startSerialTime() {
        recordTime(&startTime);
    }

    void stopSerialTime() {
        timeStruct endTime;
        recordTime(&endTime);
        serial_time = diffTime(startTime, endTime);
    }

    void setExpiry(unsigned int exp) {
        expiry = exp;
    }

    void setCommandType(cmdType c) {
        ctype = c;
    }

    char *log_name;
    unsigned long long cas;
    timeStruct startTime;
    cmdType ctype;
} mc_logger_t;
#undef __TN__

class RequestLogger : public Timer {
public:
    static RequestLogger * instance() {
        if (!m_instance) {
            m_instance = new RequestLogger();
        }
        return m_instance;
    }

    void addTime(cmdType c, uint64_t t) {
        switch (c) {
            case GET:
                getTime += t;
                break;
            case SET:
                setTime += t;
                break;
            default:
                otherTime += t;
                break;
        }
    }

    void initialize() {
        count++;
        if (!enabled) {
            return;
        }
        recordTime(&startTime);
        otherTime =
        getTime =
        setTime = 0;
    }

    void finalize(char *p) {
        if (enabled) {
            recordTime(&endTime);
            publishApacheRecord(p);
        }
    }

    void setLogOutPut(logOutPut *f) {
        enabled = true;
        out = f;
    }

    void flushConfig() {
        enabled = false;
        if (out) {
            out->close();
            delete out;
            out = NULL;
        }
    }

    std::string getID() {
        std::ostringstream out(id.str() ,std::ostringstream::app);
        out << count;
        return out.str();
    }
private:
    RequestLogger():count(0), out(NULL) {
        if ((pid = getpid()) < 0) {
            pid = 0;
        }
        id << pid << ":";
    }

    void publishApacheRecord(char *uri) {
        if (out) {
            out->write("%s %lu %lu %lu %d %.1024s", getID().c_str(), getTime, setTime,
                    getTime+setTime+otherTime, diffTime(startTime, endTime), uri);
        }
    }

    static RequestLogger *m_instance;
    timeStruct startTime, endTime;
    std::ostringstream id;
    unsigned long count, getTime, setTime, otherTime, pid;
    static bool enabled;
    logOutPut *out;
};

struct LoggerData {
    keyLoggerMap_t *logMap;
    mc_logger_t *defaultLogger;
};

class LogManager : public Timer {
public:
    LogManager(mc_logger_t *v) {
        kl = NULL;
        val = v;
        cleanData(v);
        recordTime(&startTime);
    }

    static inline void setMultiLogger() {
        kl = new keyLoggerMap_t;
    }

    static inline mc_logger_t *getLogger() {
        return val;
    }

    static inline mc_logger_t *createLogger(const char *key) {
        assert(kl);
        keyLoggerMap_t::iterator it = kl->find(key);
        if (it == kl->end() && kl->size() <= maxLoggerSize) {
            mc_logger_t *n = new mc_logger_t;
            cleanData(n);
            (*kl)[key] = n;
            n->setKey(strdup(key));
        }
    }

    static inline mc_logger_t *getLogger(const char *key) {
        keyLoggerMap_t::iterator it = kl->find(key);
        if (it != kl->end()) {
            return it->second;
        } else {
            return val;
        }
    }

    static inline void setLogger(const char *key) {
        if (kl) {
            keyLoggerMap_t::iterator it = kl->find(key);
            if (it != kl->end()) {
                val = it->second;
            }
        }
    }

    static inline void saveLogger() {
        LoggerData *l = new LoggerData;
        l->logMap = kl;
        l->defaultLogger = val;
        loggerStack.push(l);
    }

    static inline void restoreLogger() {
        LoggerData *l = loggerStack.top();
        loggerStack.pop();
        kl = l->logMap;
        val = l->defaultLogger;
        delete l;
    }

    ~LogManager() {
        recordTime(&endTime);
        uint64_t tmp = diffTime(startTime, endTime);
        RequestLogger::instance()->addTime(val->ctype, tmp);
        if (kl) {
            keyLoggerMap_t::iterator it = kl->begin();
            for (;it != kl->end(); ++it) {
                mc_logger_t *v = it->second;
                v->setResTime(tmp);
                logPublishRecord(v);
                free(v->getKey());
                delete v;
            }
            delete kl;
        } else {
            val->setResTime(tmp);
            logPublishRecord(val);
        }
    }

    static const char *checkAndLoadConfig(char *file_path);
    static bool statConfigNotChanged(char *file);
    static void logPublishRecord(mc_logger_t *d);

private:

    static void cleanData(mc_logger_t *v) {
        memset(v, 0, sizeof(mc_logger_t));
        v->setLogName("unassigned");
    }

    static keyLoggerMap_t *kl;
    static mc_logger_t *val;
    timeStruct startTime, endTime;
};


//request codes
#define MC_SUCCESS              0x0
#define MC_STORED               0x1
#define MC_UNLOCKED             200
#define MC_DELETED              201

#define MC_NOT_STORED           0x2
#define MC_NOT_FOUND            0x3
#define MC_EXISTS               0x4
#define MC_TMP_FAIL             0x5
#define MC_SERVER_MEM_ERROR     0x6
#define MC_LOCK_ERROR           0x7
#define MC_ERROR                0x8
#define MC_CLNT_ERROR           0x9
#define MC_SERVER_ERROR         0x10
#define MC_MALFORMD             0x12
#define MC_ONLY_END             0x13
#define MC_NON_NUMERIC_VALUE    0x14

#define INTERNAL_ERROR            100
#define PROXY_CONNECT_FAILED      101
#define VERSION_FAILED            102
#define POOL_NT_FOUND             103
#define SVR_NT_FOUND              104
#define READLINE_FAILED           105
#define SVR_OPN_FAILED            106
#define COMPRESS_FAILED           107
#define PARSE_ERROR               108
#define PREPARE_KEY_FAILED        109
#define INVALID_CB                110
#define INVALID_PARAM             111
#define INVALID_CAS               112
#define UNLOCKED_FAILED           113
#define FLUSH_FAILED              114
#define CLOSE_FAILED              115
#define CMD_FAILED                116
#define CONNECT_FAILED            117
#define CRC_CHKSUM1_FAILED        118
#define CRC_CHKSUM2_FAILED        119
#define CRC_CHKSUM3_FAILED        120
#define CRC_CHKSUM4_FAILED        121
#define CRC_CHKSUM5_FAILED        122
#define CRC_CHKSUM6_FAILED        123
#define CRC_CHKSUM7_FAILED        124
#define READ_VALUE_FAILED         125
#define UNCOMPRESS_FAILED         126
#define PARSE_RESPONSE_FAILED     127
#define CODE_NOT_USED              128
#define DI_CHECKSUM_GET_FAILED_GETL 129
#define DI_CHECKSUM_GET_FAILED_GET  130
#define DI_CHECKSUM_GET_FAILED1     131
#define DI_CHECKSUM_GET_FAILED2     132
#define DI_CHECKSUM_GET_FAILED3     133
#define DI_CHECKSUM_GET_FAILED4     134
#define DI_CHECKSUM_GET_FAILED5     134
#define DI_CHECKSUM_GET_FAILED_MB     135
#define DI_CHECKSUM_GET_FAILED_MCMUX     136
#define DI_CHECKSUM_GET_FAILED_MOXI     137
#define DI_CHECKSUM_SET_FAILED      138
#define DI_CHECKSUM_GET_FAILED_BYKEY  139
#define WRITE_TO_SERVER_FAILED  140
#define UNSERIALIZE_FAILED      141
#define VALUE_NOT_FOUND         142
#define NOT_STRING_DATA         143
#define READ_FROM_SERVER_FAILED 144

#endif

#if 0
request error_code
critical               0x0f
connect failed         0x01
read stream failed     0x02
write stream failed    0x04

warning:               0xf0
end line               0x10
not found              0x20
temorary failure       0x40
lock error             0x80

success                0x00
#endif

//current commands add/set/cas/get
//read config from file and set the log on the basis of rules.
//parse config only if logging is enabled.
//adding a validator
