#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <vector>
#include <string>

struct ProcessInfo {
    int pid;
    std::string command;
    double cpu_usage;
};

class ProcessMonitor {
public:
    static std::vector<ProcessInfo> getTopProcesses(int count = 5);
};

#endif 