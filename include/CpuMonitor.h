#ifndef CPUMONITOR_H
#define CPUMONITOR_H

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
public:
    // Constructeur
    CpuMonitor();
    
    // Méthode pour récupérer l'utilisation du CPU (en pourcentage 0-100)
    double getCpuUsage();
    
    // Destructeur
    ~CpuMonitor();
    
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