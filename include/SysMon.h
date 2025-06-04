#ifndef SYSMON_H
#define SYSMON_H

#include <string>
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"

class SysMon {
private:
    int updateInterval;
    bool fullLog;
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;

public:
    SysMon(int interval = 1000, bool fullLog = false);
    
    int run();
    
    std::string exportAsText();
    std::string exportAsCSV();
    
    bool update();
    
    std::string getTime();
    std::string getInfo(std::string infoType);
    void log();
};

#endif // SYSMON_H