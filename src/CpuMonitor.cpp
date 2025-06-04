#include "CpuMonitor.h"
#include <iostream>
#include <thread>
#include <chrono>

#ifdef __linux__
    #include <sstream>
#endif

using namespace std;

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
    CPU.frequencyMax = 0; // To-Do : détecter fréquence max
    CPU.nbrCPU = 0;       // To-Do : détecter nombre de CPU
}

// Méthode pour récupérer l'utilisation du CPU
float CpuMonitor::getCpuUsage() {
#ifdef _WIN32
    PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);

    return (float)counterVal.dblValue;

#elif defined(__linux__)
    CpuTimes currentTimes = readCpuTimes();

    if (firstRead) {
        lastCpuTimes = currentTimes;
        firstRead = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        currentTimes = readCpuTimes();
    }

    long long totalDiff = (currentTimes.user + currentTimes.nice + currentTimes.system +
                          currentTimes.idle + currentTimes.iowait + currentTimes.irq +
                          currentTimes.softirq + currentTimes.steal) -
                         (lastCpuTimes.user + lastCpuTimes.nice + lastCpuTimes.system +
                          lastCpuTimes.idle + lastCpuTimes.iowait + lastCpuTimes.irq +
                          lastCpuTimes.softirq + lastCpuTimes.steal);

    long long idleDiff = currentTimes.idle - lastCpuTimes.idle;

    float cpuUsage = 0.0f;
    if (totalDiff > 0) {
        cpuUsage = 100.0f * (1.0f - (float)idleDiff / (float)totalDiff);
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
        return 0.0f;
    }

    if (firstRead) {
        if (lastCpuInfo != NULL) {
            vm_deallocate(mach_task_self(), (vm_address_t)lastCpuInfo, lastNumCpuInfo);
        }
        lastCpuInfo = cpuInfo;
        lastNumCpuInfo = numCpuInfo;
        lastNumCpus = numCpus;
        firstRead = false;

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

    float cpuUsage = 0.0f;
    if (totalTicks > 0) {
        cpuUsage = 100.0f * (1.0f - (float)idleTicks / (float)totalTicks);
    }

    vm_deallocate(mach_task_self(), (vm_address_t)lastCpuInfo, lastNumCpuInfo);
    lastCpuInfo = cpuInfo;
    lastNumCpuInfo = numCpuInfo;
    lastNumCpus = numCpus;

    return cpuUsage;

#else
    // Plateforme non supportée
    return 0.0f;
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

// Mise à jour des données CPU
bool CpuMonitor::update() {
    // Exemple : ici on mettrait à jour les valeurs du struct CPU
    CPU.usageCPU  = getCpuUsage();
    CPU.frequency = getCpuFreq();
    rawCPU = getCpuInfo();
    return true;
}

// Récupération de la fréquence actuelle (stub)
float CpuMonitor::getCpuFreq() {
    // To-Do : implémenter la lecture de fréquence CPU
    return 0.0f;
}

// Récupération d'infos brutes CPU (stub)
std::string CpuMonitor::getCpuInfo() {
    // To-Do : implémenter l'extraction d'infos CPU
    return "";
}
