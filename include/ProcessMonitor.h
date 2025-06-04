// include/ProcessMonitor.h
#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>

struct AP {
    std::string user;
    float cpu;
    float memory;
    struct time {
        int hours, minutes, seconds;
    } processTime;
    std::string pathName;
};

class ProcessMonitor {
private:
    int* PID;
    AP ap;
    int nbrProcess;
public:
    bool update();
    AP getProcess(int index);
    std::string getProcessInfo();
};

#endif
