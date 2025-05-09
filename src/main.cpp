#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "..\include\CpuMonitor.h"
#include "..\include\MemoryMonitor.h"
#include "..\include\ProcessMonitor.h"
// #include "..\include\SysMon.h"
using namespace std;

int main() {
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    const int refreshInterval = 1;

    while (true) {
        cout << "Utilisation du CPU : " << cpuMonitor.getCpuUsage() << "%" << endl;
        cout << "Utilisation de la mémoire : " << memoryMonitor.getMemoryUsage() << "%" << endl;

        vector<ProcessInfo> processes = processMonitor.getProcessList();
        cout << "Processus actifs : " << endl;
        for (const auto& process : processes) { cout << "PID: " << process.pid << " - Nom: " << process.name<< " - CPU: " << process.cpuUsage << "% - RAM: " << process.memoryUsage << "%" << endl; }

        cout << "----------------------------------" << endl;
        this_thread::sleep_for(chrono::seconds(refreshInterval));
    }

    return 0;
}
