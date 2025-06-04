#pragma once
#ifndef _CPUMONITOR_H
#define _CPUMONITOR_H

#include <string>
#include <vector>

typedef struct cpu {
    float frequency;
    float frequencyMax;
    float usageCPU;
    float* usagePerCPU;
    short nbrCPU;
} cpu;

#ifdef _WIN32
    #include <windows.h>
    #include <pdh.h>
    #pragma comment(lib, "pdh.lib")
#elif defined(__linux__)
    #include <fstream>
    #include <string>
    #include <vector>
#elif defined(__APPLE__)
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <mach/mach.h>
    #include <mach/processor_info.h>
    #include <mach/mach_host.h>
#endif

// Structure commune pour les temps CPU (Linux)
struct CpuTimes {
    long long user = 0;
    long long nice = 0;
    long long system = 0;
    long long idle = 0;
    long long iowait = 0;
    long long irq = 0;
    long long softirq = 0;
    long long steal = 0;
    long long guest = 0;
    long long guest_nice = 0;

    long long totalIdleTime() const {
        return idle + iowait;
    }

    long long totalTime() const {
        return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
    }
};

class CpuMonitor {
protected:
    cpu CPU;
    std::string rawCPU;

#ifdef _WIN32
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;
#elif defined(__linux__)
    CpuTimes previousTimes;
    CpuTimes currentTimes;
    bool firstRead;
#elif defined(__APPLE__)
    processor_info_array_t lastCpuInfo;
    mach_msg_type_number_t lastNumCpuInfo;
    natural_t lastNumCpus;
    bool firstRead;
#endif

    // Méthodes privées
#if defined(__linux__)
    CpuTimes readCpuTimes();
    void updateTimes();
#endif

public:
    CpuMonitor();
    ~CpuMonitor();

    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
    bool update();
};

#endif // _CPUMONITOR_H
