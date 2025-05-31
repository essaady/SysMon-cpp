#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <vector>
#include <string>

class CpuMonitor {
public:
    CpuMonitor();

    bool readCpuStats(std::vector<unsigned long long>& stats);


    double getCpuUsage();

private:
    std::vector<unsigned long long> prevStats;
    bool firstRead;
};

#endif
