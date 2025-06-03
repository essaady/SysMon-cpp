#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class MemoryMonitor {
public:
    struct RAM {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t SwapMemInMb;
        float usageSwp;
        float freeSwp;
    } RAM;

    vector<string> memInfo;

    bool update() {
        ifstream file("/proc/meminfo");
        if (!file.is_open()) return false;

        string line;
        memInfo.clear();

        size_t memTotal = 0, memFree = 0;
        size_t swapTotal = 0, swapFree = 0;

        while (getline(file, line)) {
            memInfo.push_back(line);

            if (line.find("MemTotal:") == 0) memTotal = extractValue(line);
            else if (line.find("MemFree:") == 0) memFree = extractValue(line);
            else if (line.find("SwapTotal:") == 0) swapTotal = extractValue(line);
            else if (line.find("SwapFree:") == 0) swapFree = extractValue(line);
        }

        RAM.totalMemInMb = memTotal / 1024;
        RAM.freeMem = memFree / 1024.0;
        RAM.usage = RAM.totalMemInMb - RAM.freeMem;

        RAM.SwapMemInMb = swapTotal / 1024;
        RAM.freeSwp = swapFree / 1024.0;
        RAM.usageSwp = RAM.SwapMemInMb - RAM.freeSwp;

        return true;
    }

    unsigned long getTotalMemory() const { return RAM.totalMemInMb; }
    unsigned long getFreeMemory() const { return static_cast<unsigned long>(RAM.freeMem); }
    unsigned long getUsedMemory() const { return static_cast<unsigned long>(RAM.usage); }

    double getMemoryUsagePercentage() const {
        if (RAM.totalMemInMb == 0) return 0.0;
        return (RAM.usage / RAM.totalMemInMb) * 100.0;
    }

    unsigned long getTotalSwap() const { return RAM.SwapMemInMb; }
    unsigned long getFreeSwap() const { return static_cast<unsigned long>(RAM.freeSwp); }
    unsigned long getUsedSwap() const { return static_cast<unsigned long>(RAM.usageSwp); }

    double getSwapUsagePercentage() const {
        if (RAM.SwapMemInMb == 0) return 0.0;
        return (RAM.usageSwp / RAM.SwapMemInMb) * 100.0;
    }

private:
    size_t extractValue(const string& line) const {
        istringstream iss(line);
        string key;
        size_t value;
        string unit;
        iss >> key >> value >> unit;
        return value; // in KB
    }
};
