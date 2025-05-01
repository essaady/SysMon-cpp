#include "../include/CpuMonitor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <algorithm>

// Load Pdh.lib
#pragma comment(lib, "pdh.lib")

// Constructor
CpuMonitor::CpuMonitor() : queryHandle(nullptr), totalCpuCounter(nullptr), numCores(0) {
    // Initialize PDH counters
    if (!initPdhCounters()) {
        std::cerr << "Failed to initialize PDH counters" << std::endl;
    }
    
    // Do an initial update to populate data
    update();
}

// Destructor
CpuMonitor::~CpuMonitor() {
    // Clean up PDH resources
    if (queryHandle) {
        PdhCloseQuery(queryHandle);
    }
}

// Initialize PDH counters for CPU monitoring
bool CpuMonitor::initPdhCounters() {
    PDH_STATUS status;
    
    // Create a query for the performance data
    status = PdhOpenQuery(NULL, 0, &queryHandle);
    if (status != ERROR_SUCCESS) {
        std::cerr << "PdhOpenQuery failed with error " << status << std::endl;
        return false;
    }
    
    // Add counter for total CPU usage
    status = PdhAddEnglishCounter(queryHandle, 
                                 "\\Processor(_Total)\\% Processor Time", 
                                 0, 
                                 &totalCpuCounter);
                                 
    if (status != ERROR_SUCCESS) {
        std::cerr << "PdhAddEnglishCounter for total CPU failed with error " << status << std::endl;
        return false;
    }
    
    // Get system info to determine the number of processors
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    numCores = sysInfo.dwNumberOfProcessors;
    
    // Add counters for individual CPU cores
    cpuCoreCounters.resize(numCores);
    
    for (int i = 0; i < numCores; i++) {
        std::string counterPath = "\\Processor(" + std::to_string(i) + ")\\% Processor Time";
        
        status = PdhAddEnglishCounter(queryHandle,
                                    counterPath.c_str(),
                                    0,
                                    &cpuCoreCounters[i]);
                                    
        if (status != ERROR_SUCCESS) {
            std::cerr << "PdhAddEnglishCounter for CPU " << i << " failed with error " << status << std::endl;
            return false;
        }
    }
    
    // Initial collection to set a baseline
    status = PdhCollectQueryData(queryHandle);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Initial PdhCollectQueryData failed with error " << status << std::endl;
        return false;
    }
    
    // Need to wait a moment before collecting again to get valid data
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return true;
}

// Collect current CPU data
bool CpuMonitor::collectCpuData() {
    PDH_STATUS status;
    PDH_FMT_COUNTERVALUE counterValue;
    
    // Collect new data
    status = PdhCollectQueryData(queryHandle);
    if (status != ERROR_SUCCESS) {
        std::cerr << "PdhCollectQueryData failed with error " << status << std::endl;
        return false;
    }
    
    // Get total CPU usage
    status = PdhGetFormattedCounterValue(totalCpuCounter,
                                        PDH_FMT_DOUBLE,
                                        NULL,
                                        &counterValue);
                                       
    if (status == ERROR_SUCCESS) {
        cpuUsage["cpu"] = counterValue.doubleValue;
    } else {
        std::cerr << "PdhGetFormattedCounterValue for total CPU failed with error " << status << std::endl;
        cpuUsage["cpu"] = 0.0;
    }
    
    // Get individual core usages
    for (int i = 0; i < numCores; i++) {
        status = PdhGetFormattedCounterValue(cpuCoreCounters[i],
                                           PDH_FMT_DOUBLE,
                                           NULL,
                                           &counterValue);
                                          
        std::string cpuId = "cpu" + std::to_string(i);
        
        if (status == ERROR_SUCCESS) {
            cpuUsage[cpuId] = counterValue.doubleValue;
        } else {
            std::cerr << "PdhGetFormattedCounterValue for " << cpuId << " failed with error " << status << std::endl;
            cpuUsage[cpuId] = 0.0;
        }
    }
    
    return true;
}

// Update CPU stats and calculate usage
bool CpuMonitor::update() {
    return collectCpuData();
}

// Get CPU usage percentage for a specific CPU core
double CpuMonitor::getUsage(const std::string& cpuId) {
    auto it = cpuUsage.find(cpuId);
    if (it != cpuUsage.end()) {
        return it->second;
    }
    return 0.0;
}

// Get a map of all CPU usage percentages
std::map<std::string, double> CpuMonitor::getAllUsages() {
    return cpuUsage;
}

// Get the number of CPU cores
int CpuMonitor::getNumCores() {
    return numCores;
}

// Export CPU data to CSV format
std::string CpuMonitor::exportToCsv() {
    std::stringstream ss;
    ss << "CPU,Usage(%)\n";
    
    for (const auto& pair : cpuUsage) {
        ss << pair.first << "," << std::fixed << std::setprecision(2) << pair.second << "\n";
    }
    
    return ss.str();
}

// Export CPU data to formatted text
std::string CpuMonitor::exportToText() {
    std::stringstream ss;
    ss << "CPU Usage Report:\n";
    ss << "----------------\n";
    
    // Display total CPU usage first
    ss << "Total CPU: " << std::fixed << std::setprecision(2) << getUsage("cpu") << "%\n\n";
    
    ss << "Per Core Usage:\n";
    for (const auto& pair : cpuUsage) {
        if (pair.first != "cpu") {  // Skip the total CPU entry we already displayed
            ss << "  " << pair.first << ": " << std::fixed << std::setprecision(2) << pair.second << "%\n";
        }
    }
    
    return ss.str();
}