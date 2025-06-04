#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"

/**
 * @brief Classe principale SysMon qui regroupe tous les moniteurs
 */
class SysMon {
public:
    SysMon() = default;

    void updateAll();        // Met à jour CPU, mémoire, et processus
    std::string getReport() const;  // Rapport complet

private:
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
};

#endif
