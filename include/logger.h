#ifndef __LOGGER_H__
#define __LOGGER_H__
#include <sys/time.h>
#include <syslog.h>
enum field_type {INVAL, STRING, NUMBER};

typedef struct timeval timeStruct;

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

    void setCmd(char *ln) {
        command = ln;
    }
 
    void setFlags(int ln) {
        flags = ln;
    } 

    void setCode(int ln) {
        res_code = ln;
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

    void setCas(unsigned int c) {
        cas = c;
    }

    void startSerialTime() {
        recordTime(&startTime);
    }

    void stopSerialTime() {
        recordTime(&endTime);
        serial_time = diffTime(startTime, endTime); 
    }

    void setExpiry(unsigned int exp) {
        expiry = exp;
    }

    char *log_name;
    int cas;
    timeStruct startTime, endTime;

} mc_logger_t;
#undef __TN__

class LogManager : public Timer {
public:
    LogManager(mc_logger_t *v, bool p = true) {
        val = v;
        cleanData();
        recordTime(&startTime);
        mPublish = p;
    }

    static inline mc_logger_t *getLogger() {
        return val;
    }

    ~LogManager() {
        if (mPublish) {
            recordTime(&endTime);
            val->setResTime(diffTime(startTime, endTime));
            logPublishRecord(val);
        }
    }

    static const char *checkAndLoadConfig(char *file_path);
    static bool statConfigNotChanged(char *file); 
    static void logPublishRecord(mc_logger_t *d);   
 
private:
    void cleanData() {
        memset(val, 0, sizeof(mc_logger_t));
        val->setLogName("unassigned");
    }

    bool mPublish;
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
#define COMPRSS_FAILED            107  
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