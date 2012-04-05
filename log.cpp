#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <stdarg.h>
#include <iostream>
#include "parser.h"
#include "log.h"

#ifndef O_LARGEFILE
# define O_LARGEFILE 0
#endif

#define MAX_LOGBUF_LEN 4*1024
#include <sys/time.h>

int fileOut::open(const char *logFile) {
    if (logFile && logMode == ERRORLOG_FILE) {
        if (-1 == (logFd = ::open(logFile, O_APPEND | O_WRONLY | O_CREAT | O_LARGEFILE, 0644))) {
            fprintf(stderr, "ERROR: opening errorlog '%s' failed. error: %s, Switching to stderr.\n",
                    logFile, strerror(errno));
            logMode = ERRORLOG_STDERR;
        }
    }

    if (logMode == ERRORLOG_STDERR) {
         logFd = STDERR_FILENO;
    }
    return 0;
}

void fileOut::close() {
    switch(logMode) {
        case ERRORLOG_FILE:
            if (logFd != -1) {
                ::close(logFd);
                logFd = -1;
            }
            break;
        case ERRORLOG_STDERR:
            break;
    }
}

int fileOut::write(const char *fmt, ...) {
    va_list ap;
    LOG("in the write function");
    static char logbuf[MAX_LOGBUF_LEN];            /* scratch buffer */
    int logbuf_used = 0;                    /* length of scratch buffer */
    time_t t;
    int out;

    time(&t);
    logbuf_used += strftime(logbuf+logbuf_used, MAX_LOGBUF_LEN - 2, 
            "[%Y-%m-%d %H:%M:%S] ", localtime(&t));
    va_start(ap, fmt);
    if ((out = vsnprintf((logbuf + logbuf_used), (MAX_LOGBUF_LEN - logbuf_used - 2),
         fmt, ap)) < 0) {
        LOG("vsnprintf failed to write in buffer");
        return -1;
    }
    logbuf_used += out;
    va_end(ap);
    logbuf[logbuf_used] = '\n';
    logbuf[logbuf_used+1] = '\0';
    ::write(logFd, logbuf, logbuf_used+1);  
    return 0;
}

int syslogOut::open(const char *p) {
    openlog(LOG_IDENT_PECL, LOG_CONS | LOG_PID,  LOG_LOCAL4);
    return 0;
}

void syslogOut::close() {
    closelog();
}

int syslogOut::write(const char *fmt, ...) {
    va_list ap;
    static char logbuf[MAX_LOGBUF_LEN];            /* scratch buffer */
    int logbuf_used = 0, out = 0;                    /* length of scratch buffer */

    va_start(ap, fmt);
    if ((out = vsnprintf((logbuf + logbuf_used), (MAX_LOGBUF_LEN - logbuf_used - 2),
         fmt, ap)) < 0) {
        LOG("vsnprintf failed to write in buffer");
        return -1;
    }
    logbuf_used += std::min(out, MAX_LOGBUF_LEN - logbuf_used - 2);
    va_end(ap);

    logbuf[logbuf_used] = '\n';
    logbuf[logbuf_used+1] = '\0';
    syslog(type, "%s", logbuf);  
    return 0;
}

