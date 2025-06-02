#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>
#include <fstream>
#include <map>

struct MemoryInfo {
    unsigned long totalMemory;      // en KB
    unsigned long freeMemory;       // en KB
    unsigned long availableMemory;  // en KB
    unsigned long usedMemory;       // en KB
    unsigned long buffers;          // en KB
    unsigned long cached;           // en KB
    unsigned long totalSwap;        // en KB
    unsigned long freeSwap;         // en KB
    unsigned long usedSwap;         // en KB
    
    // Pourcentages calculés
    double memoryUsagePercent;
    double swapUsagePercent;
};

class MemoryMonitor {
private:
    std::string memInfoPath;
    std::map<std::string, unsigned long> memoryData;
    
    // Méthodes privées
    bool readMemInfo();
    unsigned long parseMemoryValue(const std::string& line);
    void calculateUsage(MemoryInfo& info);

public:
    MemoryMonitor();
    ~MemoryMonitor();
    
    // Méthodes publiques principales
    bool getMemoryInfo(MemoryInfo& info);
    void displayMemoryInfo();
    void displayMemoryInfo(const MemoryInfo& info);
    
    // Méthodes utilitaires
    std::string formatBytes(unsigned long bytes);
    bool isMemoryAvailable();
};

#endif // MEMORY_MONITOR_H