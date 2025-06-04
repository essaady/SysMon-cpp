#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <string>
using namespace std;

class SysMon {
public:
    int updateInterval;
    string exportPath;
    bool help;

    string exportAsText();
    string exportAsCSV();
    bool update(CpuMonitor, MemoryMonitor, ProcessMonitor);
    string getTime();
    string getInfo();
};

#endif
