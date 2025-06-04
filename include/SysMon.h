// include/SysMon.h
#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <string>

class SysMon {
public:
    static int updateInterval;
    static std::string exportFormat;
    static bool help;

    static std::string exportAsText();
    static std::string exportAsCSV();
    static bool update(CpuMonitor& cpu, MemoryMonitor& mem, ProcessMonitor& proc);
    static std::string getTime();
    static std::string getInfo();
};

#endif
