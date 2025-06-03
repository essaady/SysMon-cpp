#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.cpp"
#include "MemoryMonitor.cpp"
#include "ProcessMonitor.cpp"

class SysMon {
public:
    std::string exportAsJSON(CpuMonitor cpuMon, MemoryMonitor memMon, ProcessMonitor procMon);
};

#endif
