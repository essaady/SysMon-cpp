#include <iostream>
using namespace std;

#include "../include/CpuMonitor.h"
#include "../include/MemoryMonitor.h"
#include "../include/ProcessMonitor.h"

int main() {
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    
    cout << "==============Welcome to SYSMON-CPP !!!==============" << endl;
    // Print CPU infos
    cout << "CPU Model: " << cpuMonitor.getCpuModelName() << endl;
    cout << "Number of Cores: " << cpuMonitor.getCoreCount() << endl;
    cout << "CPU Usage: " << cpuMonitor.getCpuUsagePercent() << "%" << endl;
    
    return 0;
}