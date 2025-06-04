#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

class SysMon {
public:
    SysMon(int interval = 2, bool log = false);

    bool update();
    std::string getTime();
    std::string getInfo(const std::string& type);
    std::string exportAsText();
    std::string exportAsCSV();
    void log();
    int run();

private:
    int updateInterval;
    bool fullLog;
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
};

#endif
