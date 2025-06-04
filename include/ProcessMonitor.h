#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>

class ProcessMonitor {
    int* PID;
    struct AP {
        std::string user;
        float cpu;
        float memory;
        std::string time;
        std::string pathName;
    } AP;

    int nbrProcess;

public:
    bool update();
    AP getProcess(int);
    std::string getProcessInfo();
};

#endif
