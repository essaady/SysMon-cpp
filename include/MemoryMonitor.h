#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>

struct MemoryInfo {
    size_t totalPhysical;      
    size_t availablePhysical;  
    size_t processUsed;        
    size_t processPeak;        
};

class Memory {
public:
    static MemoryInfo getMemoryInfo();
    
    static size_t getTotalPhysicalMemory();
    
    static size_t getAvailablePhysicalMemory();
    
    static size_t getProcessMemoryUsage();
    
    static size_t getProcessPeakMemoryUsage();
    
    static std::string formatBytes(size_t bytes);
};

#endif 
