#pragma once
#ifndef _PROCESSMONITOR_H
#define _PROCESSMONITOR_H

#include <vector>
#include <string>

struct activeProcesses {
    int pid;
    float cpu;
    float memory;
    std::string elapsedTime;
    std::string user;
    std::string pathName;
};

class ProcessMonitor {
protected:
    std::vector<activeProcesses> processList; // store all active processes
    int nbrProcess;

public:
    ProcessMonitor();
    ~ProcessMonitor();

    bool update();
    activeProcesses getProcess(int index);
    std::string getProcessInfo();
    std::string getProcessRaw();
};

#endif
