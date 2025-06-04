#pragma once
#ifndef _PROCESSMONITOR_H
#define _PROCESSMONITOR_H

#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <iostream>
#include <algorithm>
#include <iomanip>

#define PROC_DIR "/proc/"

typedef struct ProcessInfo {
    int pid;
    float cpu;
    float memory;
    std::chrono::system_clock::time_point timestamp;
    std::string user;
    std::string command;
    std::string status;
    int threads;
    long startTime;
} ProcessInfo;

class ProcessMonitor {
private:
    std::vector<ProcessInfo> processes;
    int maxProcessesToShow;
    bool sortByCpu;
    bool sortByMemory;
    
    bool readProcessInfo(int pid, ProcessInfo& info);
    std::string getProcessUser(int uid);
    std::string getProcessCommand(int pid);
    std::string getProcessStatus(int pid);
    float calculateCpuUsage(int pid);
    float calculateMemoryUsage(int pid);
    int getProcessThreadCount(int pid);
    void sortProcesses();

public:
    ProcessMonitor(int maxProcesses = 10);
    ~ProcessMonitor();
    
    bool update();
    
    void displayProcessInfo();
    
    const std::vector<ProcessInfo>& getProcesses() const;
    
    void setSortByCpu(bool sort);
    void setSortByMemory(bool sort);
    void setMaxProcessesToShow(int count);
    
    float getTotalCpuUsage() const;
    float getTotalMemoryUsage() const;
    
    std::string exportProcessInfoAsText() const;
    std::string exportProcessInfoAsCSV() const;
};

#endif 