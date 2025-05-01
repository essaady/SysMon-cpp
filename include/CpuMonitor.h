#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <string>
#include <vector>
#include <map>
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>

class CpuMonitor {
private:
    // PDH query handle
    PDH_HQUERY queryHandle;
    
    // Counter handles for total CPU and individual cores
    PDH_HCOUNTER totalCpuCounter;
    std::vector<PDH_HCOUNTER> cpuCoreCounters;
    
    // CPU usage data
    std::map<std::string, double> cpuUsage;
    
    // Number of processor cores
    int numCores;
    
    // Initialize PDH query and counters
    bool initPdhCounters();
    
    // Collect CPU data
    bool collectCpuData();

public:
    CpuMonitor();
    ~CpuMonitor();

    // Update CPU stats and calculate usage
    bool update();
    
    // Get CPU usage percentage for a specific CPU core (or "cpu" for total)
    double getUsage(const std::string& cpuId = "cpu");
    
    // Get a map of all CPU usage percentages
    std::map<std::string, double> getAllUsages();
    
    // Get the number of CPU cores
    int getNumCores();
    
    // Export CPU data to CSV format
    std::string exportToCsv();
    
    // Export CPU data to formatted text
    std::string exportToText();
};

#endif // CPU_MONITOR_H