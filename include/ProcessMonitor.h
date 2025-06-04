#pragma once
#ifndef _PROCESSMONITOR_H
#define _PROCESSMONITOR_H

#include <vector>
#include <string>
#include <sstream>

typedef struct activeProcesses {
    float cpu;               // CPU usage per process
    float memory;           // Memory usage per process
    struct time;            // Process time information
    std::string user;       // Process owner
    std::string pathName;   // Process executable path
    int* PID;              // Process IDs array
    int nbrProcess;        // Number of active processes
} activeProcesses;



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