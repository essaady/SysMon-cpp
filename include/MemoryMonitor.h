#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <iostream>

class MemoryMonitor {
public:
    MemoryMonitor();
    bool update();
    void displayUsage();

private:
    double totalMemMb;
    double usedMemMb;
};

#endif
