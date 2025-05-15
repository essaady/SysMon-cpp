#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>

class MemoryMonitor {
public:
    MemoryMonitor();
    ~MemoryMonitor();

    bool update();
    double getTotalMemory() const;
    double getUsedMemory() const;
    double getFreeMemory() const;
    std::string exportAsText() const;
    std::string exportAsCSV() const;

private:
    double totalMem;
    double freeMem;
    double usedMem;

    bool readMemInfo();
};

#endif // MEMORY_MONITOR_H
