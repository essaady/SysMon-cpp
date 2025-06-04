#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>

class CpuMonitor {
public:
    CpuMonitor() = default;
    void update();              // Met à jour les données CPU
    float getCpuUsage() const;  // Retourne l'utilisation CPU en %
    std::string getCpuInfo() const; // Informations générales CPU

private:
    float cpuUsage = 0.0f;
};

#endif
