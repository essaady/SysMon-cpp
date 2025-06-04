#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <string>
#include <vector>

class ram {
private:
    size_t totalMemInMb;
    float usage;
    float freeMem;
    size_t SwapMeminMb;
    float usageSwp;
    float freeSwp;
};

class MemoryMonitor {
private:
    ram RAM;
    std::vector<std::string> memInfo;
};

#endif 
