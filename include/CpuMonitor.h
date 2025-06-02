#ifndef CPUMONITOR_H
#define CPUMONITOR_H
#include <vector>
#include <string>
#include <chrono>

// Structure to hold CPU time values read from /proc/stat
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
public:
    CpuMonitor();
    // Calculates and returns the current CPU usage percentage
    double getCpuUsage(); 

private:
    CpuTimes previousTimes;
    CpuTimes currentTimes;

    CpuTimes readCpuTimes(); // read proc/stat
    void updateTimes();      // Updates  times
};

#endif 
