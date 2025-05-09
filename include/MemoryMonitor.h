#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>

class MemoryMonitor {
    private:
        long totalMemory;       // En kilooctets
        long availableMemory;   // En kilooctets

    public:
        void update();               
        std::string getUsageInfo();  
};

#endif
