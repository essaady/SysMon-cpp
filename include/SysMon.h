#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"

// Classe SysMon qui regroupe tous les moniteurs
class SysMon {
public:
    SysMon();
    
    // Met à jour tous les moniteurs
    void update();

    // Renvoie les moniteurs
    CpuMonitor& getCpuMonitor();
    MemoryMonitor& getMemoryMonitor();
    ProcessMonitor& getProcessMonitor();

private:
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
};

#endif // SYSMON_H
