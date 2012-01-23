#include<iostream>
#include<fstream>
#include"parser.h"
#include <sys/stat.h>
#include <unistd.h>
using namespace std;

string errorString;
static mc_logger_t record; 

bool LogManager::statConfigNotChanged(char *file) {
    static struct stat fileStat;
    static time_t lastTime;
    stat(file, &fileStat);
    LOG("ctime is %d mitme is %d lastime %d", fileStat.st_ctime, 
        fileStat.st_ctime, lastTime);
    if (fileStat.st_ctime != lastTime) {
        LOG("config changed");
        lastTime = fileStat.st_ctime;
        return false;
    } 
    return true; 
}

const char *LogManager::checkAndLoadConfig(char *p) {
    if (statConfigNotChanged(p)) {
        return NULL;
    }
    /*flush old config*/
    logger::instance()->flushConfig();   

    if (p == NULL) {
        LOG("config file is NULL");
        return NULL;
    }
    ifstream configFile(p);
    string str;
    errorString = "";
    char mcMaxName[100];
    if (configFile.is_open()) {
        while (configFile.good()) {
            getline(configFile, str);
            if (sscanf(str.c_str(), "%s", 
                        mcMaxName) > 0) {
                exprParser *ptr = NULL;
                LOG("client name is %s", mcMaxName);
                getline(configFile, str);
                if (str.size() > 0) {
                    ptr = new exprParser(str);
                    if (!ptr->buildTree()) {
                        errorString += str;
                        errorString += "\n"; 
                        delete ptr;
                        ptr = NULL;
                    }
                    else {
                        logger *p = logger::instance();
                        if (!p->insertExprTree(mcMaxName, ptr)) {
                            delete ptr;
                            ptr = NULL;
                        }
                    }
                }  
                getline(configFile, str);
                if (ptr) {
                    if (str.size() > 0) {
                        logOutPut *p;
                        if (!strncasecmp(str.c_str(), "SYSLOG", 6)) {
                            p = new syslogOut();
                        }
                        else {
                            p = new fileOut();
                        } 
                        if (!p->open(str.c_str())) { 
                            ptr->setLogOutPut(p); 
                        }
                        else {
                            free (p);
                            string nf("Not able to open the file : ");
                            errorString += nf;  
                            errorString += str;  
                            errorString += "\n";  
                            LOG("unable to open the log file %s", str.c_str());
                        }   
                    } else {
                        string nf("Not a valid log output \n");
                        errorString += nf;  
                    }                
                }
            }      
        }
    }
    return errorString.size() > 0 ? errorString.c_str():NULL;
}

void LogManager::logPublishRecord(mc_logger_t *d) {
    exprParser *exPar;
    if (!d) {
        return;
    }
    if ((exPar = logger::instance()->getExprTree(d->log_name)) &&
            exPar->evaluateTree((data *)d, exPar->getRoot())) {
        logOutPut *p = NULL; 
        LOG("Rule evaluated to true");   
        if ((p = exPar->getLogOutPut())) {
            LOG("Writing to output");  
            p->write("%s %s %s %s %d %d %d %d %d %d %d", d->log_name, d->host, d->command, d->key, d->res_len, d->res_code, d->flags, d->expiry, d->cas, d->res_time, d->serial_time);
        }
    }
}

