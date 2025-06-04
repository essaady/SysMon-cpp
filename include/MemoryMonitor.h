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
    bool update() {
        ifstream memFile("/proc/meminfo");
        if (!memFile.is_open()) return false;

        string line;
        ram.memInfo.clear();

        while (getline(memFile, line)) {
            ram.memInfo.push_back(line);
        }
        memFile.close();

        for (const auto& entry : ram.memInfo) {
            istringstream iss(entry);
            string key;
            size_t value;
            string unit;

            iss >> key >> value >> unit;

            if (key == "MemTotal:") totalMemMb = value / 1024;
            else if (key == "MemFree:") freeMem = value / 1024.0;
            else if (key == "SwapTotal:") SwapMemMb = value / 1024;
            else if (key == "SwapFree:") freeSwap = value / 1024.0;
        }

        return true;
    }

    // -------------------------------
    // Méthodes d'accès
    // -------------------------------
    unsigned long long getTotalMemory() const {
        return totalMemMb;
    }

    unsigned long long getFreeMemory() const {
        return static_cast<unsigned long long>(freeMem);
    }

    unsigned long long getTotalSwap() const {
        return SwapMemMb;
    }

    unsigned long long getUsedSwap() const {
        return static_cast<unsigned long long>(SwapMemMb - freeSwap);
    }

    double getMemoryUsagePercentage() const {
        return ((totalMemMb - freeMem) / totalMemMb) * 100.0;
    }

    double getSwapUsagePercentage() const {
        if (SwapMemMb == 0) return 0.0;
        return ((SwapMemMb - freeSwap) / SwapMemMb) * 100.0;
    }
};

#endif
