#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>

struct activeProcesses {
    float cpu;
    float memory;
    struct {
        int hours;
        int minutes;
        int seconds;
    } time;
    std::string user;
    std::string pathName;
};

class ProcessMonitor {
private:
    int* PID;
    std::map<int, activeProcesses> AP; // Active Processes
    int nbrProcess;

public:
    ProcessMonitor();
    ~ProcessMonitor();
    
    // Core methods
    bool update();
    std::map<int, activeProcesses> getProcess(int limit = 10);
    std::string getProcessInfo();
    std::string getProcessRaw();
    
    // Getters
    int getProcessCount() const { return nbrProcess; }
    std::map<int, activeProcesses> getAllProcesses() const { return AP; }
    
    // Utility methods
    std::vector<std::pair<int, activeProcesses>> getTopProcessesByCpu(int limit = 10);
    std::vector<std::pair<int, activeProcesses>> getTopProcessesByMemory(int limit = 10);
};

#endif 