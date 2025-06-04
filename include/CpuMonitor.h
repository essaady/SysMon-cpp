#pragma once
#ifndef _CPUMONITOR_H
#define _CPUMONITOR_H
#include <string>

typedef struct cpu{
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

class CpuMonitor {
protected:
    cpu CPU;
    std::string rawCPU;

public:
    // Constructeur
    CpuMonitor();

    // Destructeur
    ~CpuMonitor();

    // Méthode pour récupérer l'utilisation du CPU
    float getCpuUsage();

    // Méthode pour récupérer la fréquence actuelle du CPU
    float getCpuFreq();

    // Méthode pour récupérer des informations brutes du CPU
    std::string getCpuInfo();

    // Mettre à jour les données CPU
    bool update();

private:
#ifdef _WIN32
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;
#elif defined(__linux__)
    struct CpuTimes {
        long long user, nice, system, idle, iowait, irq, softirq, steal;
    };
    CpuTimes lastCpuTimes;
    bool firstRead;

    // Méthode privée pour lire les temps CPU depuis /proc/stat
    CpuTimes readCpuTimes();
#elif defined(__APPLE__)
    processor_info_array_t lastCpuInfo;
    mach_msg_type_number_t lastNumCpuInfo;
    natural_t lastNumCpus;
    bool firstRead;
#endif
};

#endif // CPUMONITOR_H
