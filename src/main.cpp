#include <iostream>
#include <iomanip>
#include <windows.h>
#include <cstdlib>
#include <string>
using namespace std;

#include "../include/SysMon.h"
#include "../include/CpuMonitor.h"
#include "../include/MemoryMonitor.h"
#include "../include/ProcessMonitor.h"

string getProgressBar(double percentage, int barWidth = 20) {
    int pos = barWidth * percentage / 100.0;
    string bar = "[";
    
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) bar += "+";
        else if (i == pos) bar += ">";
        else bar += "-";
    }
    bar += "]";
    return bar;
}

void clearScreen() {
    system("cls");
}

int main() {
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    
    // Print static information only once
    cout << "==============Welcome to SYSMON-CPP !!!==============" << endl;
    cout << "CPU Model: " << cpuMonitor.getCpuModelName() << endl;
    cout << "Physical Cores: " << cpuMonitor.getPhysicalCoreCount() << endl;
    cout << "Logical Processors: " << cpuMonitor.getLogicalProcessorCount() << endl;
    cout << "\nPress Ctrl+C to exit..." << endl << endl;

    // Continuously update CPU usage with progress bar
    while(true) {
        double usage = cpuMonitor.getCpuUsagePercent();
        double uptime = cpuMonitor.getUptime();
        
        int hours = static_cast<int>(uptime / 3600);
        int minutes = static_cast<int>((uptime - hours * 3600) / 60);
        int seconds = static_cast<int>(uptime - hours * 3600 - minutes * 60);
        
        cout << "\rCPU Usage: " << getProgressBar(usage) << " " << fixed << setprecision(2) << usage << "% ";
        cout << "| Uptime: " << setfill('0') << setw(2) << hours << ":" 
             << setfill('0') << setw(2) << minutes << ":" 
             << setfill('0') << setw(2) << seconds << " ";
        cout.flush();
        Sleep(1000); // Update every second
    }
    
    return 0;
}