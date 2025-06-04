#include "ProcessMonitor.h"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unistd.h>

struct ProcInfo {
    int pid;
    std::string cmd;
    float cpuUsage;
};

static float parseCpuUsage(const std::string& statLine) {
    std::istringstream iss(statLine);
    std::string dummy;
    int skip = 13;
    long utime, stime;
    for (int i = 0; i < skip; ++i)
        iss >> dummy;
    iss >> utime >> stime;
    return (float)(utime + stime) / sysconf(_SC_CLK_TCK);
}

void ProcessMonitor::displayTopProcesses() {
    DIR* proc = opendir("/proc");
    if (!proc) return;

    std::vector<ProcInfo> processes;
    struct dirent* entry;

    while ((entry = readdir(proc)) != nullptr) {
        if (!isdigit(entry->d_name[0])) continue;
        int pid = atoi(entry->d_name);
        std::string statPath = std::string("/proc/") + entry->d_name + "/stat";
        std::ifstream statFile(statPath);
        if (!statFile.is_open()) continue;
        std::string statLine;
        getline(statFile, statLine);
        statFile.close();
        auto start = statLine.find('(');
        auto end = statLine.find(')');
        std::string cmd = statLine.substr(start+1, end-start-1);
        float cpu = parseCpuUsage(statLine);
        processes.push_back({pid, cmd, cpu});
    }
    closedir(proc);

    std::sort(processes.begin(), processes.end(),
        [](const ProcInfo& a, const ProcInfo& b) {
            return a.cpuUsage > b.cpuUsage;
        });

    std::cout << "Top 5 processes by CPU usage:\n";
    std::cout << "PID\tCPU %\tCOMMAND\n";
    for (int i = 0; i < 5 && i < (int)processes.size(); ++i) {
        std::cout << processes[i].pid << "\t" 
                  << processes[i].cpuUsage << "\t" 
                  << processes[i].cmd << "\n";
    }
}
