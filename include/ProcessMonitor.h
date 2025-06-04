#pragma once
#ifndef _PROCESSMONITOR_H
#define _PROCESSMONITOR_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>

#define PROC_PATH "/proc/"

typedef struct ap {
    int pid;
    float cpu;
    float memory;
    std::string user;
    std::string command;
} activeProcesses;

class ProcessMonitor {
private:
    std::vector<activeProcesses> processes;
    
    bool parseProcessInfo(int pid, activeProcesses& process);
    std::string getProcessUser(int pid);
    float getProcessCpuUsage(int pid);
    float getProcessMemoryUsage(int pid);

public:
    ProcessMonitor();
    ~ProcessMonitor();
    
    bool update();
    
    const std::vector<activeProcesses>& getProcesses() const;
    
    void displayProcessInfo(int limit = 10);
};

#endif