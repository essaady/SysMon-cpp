#pragma once
#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <vector>
#include <string>

typedef struct cpu {
    float frequency;
    float frequencyMax;
    float usageCPU;
    float* usagePerCPU;
    short nbrCPU;
} cpu;

// Structure to hold CPU time values read from /proc/stat
typedef struct CpuTimes {
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
} CpuTimes;

class CpuMonitor {
private:
    CpuTimes previousTimes;
    CpuTimes currentTimes;
    cpu CPU;
    std::string rawCPU;
    std::vector<unsigned long long> prevStats;
    bool firstRead;

    CpuTimes readCpuTimes(); // read from /proc/stat
    void updateTimes();

public:
    CpuMonitor();          // Constructeur
    ~CpuMonitor();         // Destructeur

    // Pour récupérer les stats de /proc/stat
    bool readCpuStats(std::vector<unsigned long long>& stats);

    // Calcule l'utilisation CPU entre deux lectures
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
    bool update();
};

#endif
