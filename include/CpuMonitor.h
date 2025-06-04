#pragma once
#ifndef _CPUMONITOR_H
#define _CPUMONITOR_H
#include <string>

typedef struct cpu {
    float frequency;      // Current CPU frequency
    float frequencyMax;   // Maximum CPU frequency
    float usageCPU;      // Total CPU usage percentage
    float* usagePerCPU;  // Per-core CPU usage array
    short nbrCPU;        // Number of CPU cores
    std::string rawCPU;  // Raw CPU information
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
        cpu CPU;                  // CPU metrics structure
        CpuTimes previousTimes;   // Previous CPU time readings
        CpuTimes currentTimes;    // Current CPU time readings
        std::string rawCPU;       // Raw CPU information string
        
        CpuTimes readCpuTimes(); // Read CPU times from /proc/stat
        void updateTimes();      // Update CPU time readings

    public:
        CpuMonitor();
        ~CpuMonitor();

        // Get current CPU usage as a percentage
        float getCpuUsage();
        
        float getCpuFreq();

        std::string getCpuInfo();

        bool update();
};

#endif 
