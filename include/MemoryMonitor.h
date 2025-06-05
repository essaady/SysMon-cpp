#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class MemoryMonitor {
private:
    size_t totalMemMb;
    float freeMem;
    size_t SwapMemMb;
    float freeSwap;

    struct RAM {
        vector<string> memInfo; // raw mem info li ja mn /proc/meminfo
    } ram;

public:
    MemoryMonitor() : totalMemMb(0), freeMem(0), SwapMemMb(0), freeSwap(0) {}

    // -------------------------------
    // Méthode principale: update()
    // -------------------------------
    bool update();

    // -------------------------------
    // Méthodes d'accès
    // -------------------------------
    unsigned long long getTotalMemory() const;
    unsigned long long getFreeMemory() const;
    unsigned long long getTotalSwap() const;
    unsigned long long getUsedSwap() const;
    double getMemoryUsagePercentage() const;
    double getSwapUsagePercentage() const;

    // -------------------------------
    // Nouvelle méthode : Utilisation du disque
    // -------------------------------
    void afficherUtilisationDisque(const std::string& chemin = "/");
};

#endif

