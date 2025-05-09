#define _DEFAULT_SOURCE
using namespace std;

#include "..\include\ProcessMonitor.h"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <ctime>     
#include <iostream>
#include <cstring>
#include <algorithm>
#include <windows.h>

vector<ProcessInfo> ProcessMonitor::getProcessList() {
    vector<ProcessInfo> processList;
    DIR* dir = opendir("/proc");
    if (!dir) return processList;

    unsigned long totalMem = getTotalMemory();

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        string pidStr = entry->d_name;

        if (all_of(pidStr.begin(), pidStr.end(), ::isdigit)) {
            int pid = stoi(pidStr);
            string name;
            ifstream cmdline("/proc/" + pidStr + "/comm");
            getline(cmdline, name);

            if (!name.empty()) {
                ProcessInfo p;
                p.pid = pid;
                p.name = name;
                p.cpuUsage = getCpuUsageForProcess(pid); 
                p.memoryUsage = getMemoryUsageForProcess(pid, totalMem);
                processList.push_back(p);
            }
        }
    }
    closedir(dir);
    return processList;
}

float ProcessMonitor::getCpuUsageForProcess(int pid) {
    ifstream stat("/proc/" + to_string(pid) + "/stat");
    string value;
    vector<string> values;

    while (stat >> value) { values.push_back(value); }
    if (values.size() < 17) return 0.0f;

    unsigned long utime = stoul(values[13]);
    unsigned long stime = stoul(values[14]);
    unsigned long total_time = utime + stime;

    long ticks_per_sec = CLK_TCK;  
    return (float)total_time / ticks_per_sec; 
}

unsigned long ProcessMonitor::getTotalMemory() {
    ifstream file("/proc/meminfo");
    string line;
    unsigned long totalMemory = 0;
    
    while (getline(file, line)) {
        if (line.find("MemTotal:") == 0) {
            istringstream ss(line);
            string key;
            unsigned long value;
            ss >> key >> value;
            totalMemory = value;
            break;
        }
    }
    return totalMemory;
}

float ProcessMonitor::getMemoryUsageForProcess(int pid, unsigned long memTotal) {

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    unsigned long pageSize = sysInfo.dwPageSize;

    ifstream file("/proc/" + to_string(pid) + "/statm");
    unsigned long size, resident;
    file >> size >> resident;

    return (float)(resident * pageSize) / (memTotal * 1024) * 100;
}