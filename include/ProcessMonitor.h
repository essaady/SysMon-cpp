#pragma once
#ifndef _PROCESSMONITOR_H
#define _PROCESSMONITOR_H

#include <vector>
#include <string>
#include <sstream>

struct activeProcesses {
    int pid;
    float cpu;
    float memory;
    std::string elapsedTime;  // Replacing `struct time` with readable field
    std::string user;
    std::string pathName;
    std::vector<activeProcesses> processList; //Internal process list is missing

};




class ProcessMonitor{
protected:
    activeProcesses AP;
    int nbrProcess;

public:
    
    ProcessMonitor();

    ~ProcessMonitor();

    bool update();

    activeProcesses getProcess(int);
    
    std::string getProcessInfo();

    std::string getProcessRaw();

};

#endif