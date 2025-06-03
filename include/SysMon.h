#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"

extern int updateInterval;
extern std::string exportFormat;
extern bool help;

std::string exportAsText();
std::string exportAsCSV();
bool update(CpuMonitor&, MemoryMonitor&, ProcessMonitor&);
std::string getTime();
std::string getInfo();

#endif
