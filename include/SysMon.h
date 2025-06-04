#ifndef SYSMON_H
#define SYSMON_H

#include <string>
#include <vector>
#include <ctime>
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"

using namespace std;
extern int updateInterval;
extern string exportPath;
extern bool help;
string exportAsText();
string exportAsCSV();
bool update(CpuMonitor cpuMon, MemoryMonitor memMon, ProcessMonitor procMon);
string getTime();
string getInfo();
#endif 
