#include "CpuMonitor.h"
#include <iostream>
#include <thread>
#include <chrono>

#ifdef __linux__
    #include <sstream>
#endif

// Constructeur
CpuMonitor::CpuMonitor() {
#ifdef _WIN32
    PdhOpenQuery(NULL, NULL, &cpuQuery);
    PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
#elif defined(__linux__)
    firstRead = true;
    lastCpuTimes = {0, 0, 0, 0, 0, 0, 0, 0};
#elif defined(__APPLE__)
    firstRead = true;
    lastCpuInfo = NULL;
    lastNumCpuInfo = 0;
    lastNumCpus = 0;
#endif
}

// Méthode pour récupérer l'utilisation du CPU
double CpuMonitor::getCpuUsage() {
#ifdef _WIN32
    PDH_FMT_COUNTERVALUE counterVal;
    
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    
    return counterVal.dblValue;
    
#elif defined(__linux__)
    CpuTimes currentTimes = readCpuTimes();
    
    if (firstRead) {
        lastCpuTimes = currentTimes;
        firstRead = false;
        // Attendre un peu pour avoir une mesure significative
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        currentTimes = readCpuTimes();
    }
    
    // Calculer les différences
    long long totalDiff = (currentTimes.user + currentTimes.nice + currentTimes.system + 
                          currentTimes.idle + currentTimes.iowait + currentTimes.irq + 
                          currentTimes.softirq + currentTimes.steal) -
                         (lastCpuTimes.user + lastCpuTimes.nice + lastCpuTimes.system + 
                          lastCpuTimes.idle + lastCpuTimes.iowait + lastCpuTimes.irq + 
                          lastCpuTimes.softirq + lastCpuTimes.steal);
    
    long long idleDiff = currentTimes.idle - lastCpuTimes.idle;
    
    double cpuUsage = 0.0;
    if (totalDiff > 0) {
        cpuUsage = 100.0 * (1.0 - (double)idleDiff / (double)totalDiff);
    }
    
    lastCpuTimes = currentTimes;
    return cpuUsage;
    
#elif defined(__APPLE__)
    natural_t numCpus;
    processor_info_array_t cpuInfo;
    mach_msg_type_number_t numCpuInfo;
    
    kern_return_t kr = host_processor_info(mach_host_self(), PROCESSOR_CPU_LOAD_INFO, 
                                          &numCpus, &cpuInfo, &numCpuInfo);
    
    if (kr != KERN_SUCCESS) {
        return 0.0;
    }
    
    if (firstRead) {
        if (lastCpuInfo != NULL) {
            vm_deallocate(mach_task_self(), (vm_address_t)lastCpuInfo, lastNumCpuInfo);
        }
        lastCpuInfo = cpuInfo;
        lastNumCpuInfo = numCpuInfo;
        lastNumCpus = numCpus;
        firstRead = false;
        
        // Attendre un peu pour avoir une mesure significative
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return getCpuUsage();
    }
    
    natural_t *inUseSumOld = (natural_t*)lastCpuInfo;
    natural_t *inUseSumNew = (natural_t*)cpuInfo;
    
    natural_t totalTicks = 0;
    natural_t idleTicks = 0;
    
    for (natural_t i = 0; i < numCpus; i++) {
        natural_t *cpuOld = &inUseSumOld[CPU_STATE_MAX * i];
        natural_t *cpuNew = &inUseSumNew[CPU_STATE_MAX * i];
        
        natural_t userDiff = cpuNew[CPU_STATE_USER] - cpuOld[CPU_STATE_USER];
        natural_t sysDiff = cpuNew[CPU_STATE_SYSTEM] - cpuOld[CPU_STATE_SYSTEM];
        natural_t niceDiff = cpuNew[CPU_STATE_NICE] - cpuOld[CPU_STATE_NICE];
        natural_t idleDiff = cpuNew[CPU_STATE_IDLE] - cpuOld[CPU_STATE_IDLE];
        
        totalTicks += userDiff + sysDiff + niceDiff + idleDiff;
        idleTicks += idleDiff;
    }
    
    double cpuUsage = 0.0;
    if (totalTicks > 0) {
        cpuUsage = 100.0 * (1.0 - (double)idleTicks / (double)totalTicks);
    }
    
    // Nettoyer l'ancienne info et sauvegarder la nouvelle
    vm_deallocate(mach_task_self(), (vm_address_t)lastCpuInfo, lastNumCpuInfo);
    lastCpuInfo = cpuInfo;
    lastNumCpuInfo = numCpuInfo;
    lastNumCpus = numCpus;
    
    return cpuUsage;
    
#else
    // Plateforme non supportée
    return 0.0;
#endif
}

#ifdef __linux__
// Méthode privée pour lire les temps CPU depuis /proc/stat
CpuMonitor::CpuTimes CpuMonitor::readCpuTimes() {
    std::ifstream file("/proc/stat");
    std::string line;
    CpuTimes times = {0, 0, 0, 0, 0, 0, 0, 0};
    
    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string cpu;
        iss >> cpu >> times.user >> times.nice >> times.system >> times.idle 
            >> times.iowait >> times.irq >> times.softirq >> times.steal;
    }
    
    return times;
}
#endif

// Destructeur
CpuMonitor::~CpuMonitor() {
#ifdef _WIN32
    PdhCloseQuery(cpuQuery);
#elif defined(__APPLE__)
    if (lastCpuInfo != NULL) {
        vm_deallocate(mach_task_self(), (vm_address_t)lastCpuInfo, lastNumCpuInfo);
    }
#endif
}