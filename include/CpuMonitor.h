#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <vector>
#include <string>

class CpuMonitor {
public:
    CpuMonitor();
    // Lit les stats CPU (user, nice, system, idle, etc.)
    bool readCpuStats(std::vector<unsigned long long>& stats);

    // Calcule l'utilisation CPU entre deux lectures
    double getCpuUsage();

private:
    std::vector<unsigned long long> prevStats;
    bool firstRead;
};

#endif // CPUMONITOR_H
