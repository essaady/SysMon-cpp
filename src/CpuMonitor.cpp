#include "../include/CpuMonitor.h"
#include "../include/SysMon.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <pdh.h>
#pragma comment(lib, "pdh.lib")
#else
#include <dirent.h>
#endif

using namespace std;

CpuMonitor::CpuMonitor() {
    cpu CPU = {};  // Initialize all members to 0
    
#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    CPU.nbrCPU = sysInfo.dwNumberOfProcessors;
#else
    // Count number of CPU cores on Linux
    DIR* dir = opendir("/sys/devices/system/cpu");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_type == DT_DIR) {
                if (strncmp(entry->d_name, "cpu", 3) == 0) {
                    char* endptr;
                    strtol(entry->d_name + 3, &endptr, 10);
                    if (*endptr == '\0') {
                        CPU.nbrCPU++;
                    }
                }
            }
        }
        closedir(dir);
    }
#endif

    // Allocate array for per-CPU usage
    CPU.usagePerCPU = new float[CPU.nbrCPU]();
    
    // Get initial CPU times
    updateTimes();
    previousTimes = currentTimes;
    
    // Read max frequency
    string freqMax = SysMon::getInfo("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (!freqMax.empty()) {
        CPU.frequencyMax = stof(freqMax) / 1000.0; // Convert to MHz
    }

CpuMonitor::~CpuMonitor() {
    delete[] CPU.usagePerCPU;
}

CpuTimes CpuMonitor::readCpuTimes() {
    CpuTimes times;
    string statContent = SysMon::getInfo("/proc/stat");
    istringstream iss(statContent);
    string line;
    
    // Read the first line which contains total CPU stats
    getline(iss, line);
    istringstream cpuline(line);
    string cpu;
    cpuline >> cpu; // Skip "cpu" label
    
    // Read CPU time values
    cpuline >> times.user >> times.nice >> times.system >> times.idle
            >> times.iowait >> times.irq >> times.softirq >> times.steal
            >> times.guest >> times.guest_nice;
            
    return times;
}

void CpuMonitor::updateTimes() {
    previousTimes = currentTimes;
    currentTimes = readCpuTimes();
}

float CpuMonitor::getCpuUsage() {
#ifdef _WIN32
    static PDH_HQUERY cpuQuery = NULL;
    static PDH_HCOUNTER cpuTotal = NULL;
    
    if (cpuQuery == NULL) {
        // Initialize the query on first call
        PdhOpenQuery(NULL, NULL, &cpuQuery);
        PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Need two samples
    }
    
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    CPU.usageCPU = static_cast<float>(counterVal.doubleValue);
#else
    updateTimes();
    
    long long prevIdle = previousTimes.totalIdleTime();
    long long currIdle = currentTimes.totalIdleTime();
    
    long long prevTotal = previousTimes.totalTime();
    long long currTotal = currentTimes.totalTime();
    
    // Calculate deltas
    long long totalDelta = currTotal - prevTotal;
    long long idleDelta = currIdle - prevIdle;
    
    if (totalDelta == 0) return CPU.usageCPU;  // Return previous value if no change
    
    // Calculate CPU usage percentage
    CPU.usageCPU = 100.0 * (1.0 - static_cast<float>(idleDelta) / totalDelta);
#endif
    return CPU.usageCPU;
}

float CpuMonitor::getCpuFreq() {
#ifdef _WIN32
    PROCESSOR_POWER_INFORMATION* ppi = new PROCESSOR_POWER_INFORMATION[CPU.nbrCPU];
    if (CallNtPowerInformation(ProcessorInformation, NULL, 0, ppi, sizeof(PROCESSOR_POWER_INFORMATION) * CPU.nbrCPU) == STATUS_SUCCESS) {
        float totalFreq = 0.0;
        for (int i = 0; i < CPU.nbrCPU; i++) {
            totalFreq += ppi[i].CurrentMhz;
        }
        CPU.frequency = totalFreq / CPU.nbrCPU;
        CPU.frequencyMax = ppi[0].MaxMhz;
    }
    delete[] ppi;
#else
    float totalFreq = 0.0;
    int coreCount = 0;
    
    // Read current frequency for each core
    for(int i = 0; i < CPU.nbrCPU; i++) {
        string freqPath = "/sys/devices/system/cpu/cpu" + to_string(i) + "/cpufreq/scaling_cur_freq";
        string freqContent = SysMon::getInfo(freqPath);
        
        if (!freqContent.empty()) {
            float coreFreq = stof(freqContent) / 1000.0; // Convert to MHz
            totalFreq += coreFreq;
            coreCount++;
        }
    }
    
    CPU.frequency = (coreCount > 0) ? (totalFreq / coreCount) : 0.0;
#endif
    return CPU.frequency;
}

string CpuMonitor::getCpuInfo() {
    float usage = getCpuUsage();
    float freq = getCpuFreq();
    
    stringstream ss;
    ss << fixed << setprecision(1);
    ss << "\x1b[42mCPU Usage: " << usage << "% @ " << freq << " MHz";
    if (CPU.frequencyMax > 0) {
        ss << " (Max: " << CPU.frequencyMax << " MHz)";
    }
    ss << "\x1b[0m\n";
    
    // Get per-core information from /proc/stat
    string statContent = SysMon::getInfo("/proc/stat");
    istringstream iss(statContent);
    string line;
    
    // Skip the first line (total CPU)
    getline(iss, line);
    
    // Process each CPU core
    for(int coreNum = 0; coreNum < CPU.nbrCPU && getline(iss, line); coreNum++) {
        if(line.find("cpu") != 0) break;
        
        istringstream coreline(line);
        string cpu;
        CpuTimes coreTimes;
        
        coreline >> cpu; // Skip "cpuN" label
        coreline >> coreTimes.user >> coreTimes.nice >> coreTimes.system 
                 >> coreTimes.idle >> coreTimes.iowait >> coreTimes.irq 
                 >> coreTimes.softirq >> coreTimes.steal;
                 
        long long totalTime = coreTimes.totalTime();
        long long idleTime = coreTimes.totalIdleTime();
        CPU.usagePerCPU[coreNum] = 100.0 * (1.0 - static_cast<float>(idleTime) / totalTime);
        
        ss << "Core " << coreNum << ": " << CPU.usagePerCPU[coreNum] << "%\n";
    }
    
    return ss.str();
}

bool CpuMonitor::update() {
    try {
        // Update all CPU metrics
        float usage = getCpuUsage();
        float freq = getCpuFreq();
        string info = getCpuInfo();
        
        // Only print if not in a logging mode
        cout << info;
        
        return true;
    } catch (const exception& e) {
        cerr << "Error updating CPU metrics: " << e.what() << endl;
        return false;
    }
}

