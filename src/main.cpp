#include <iostream>
#include <unistd.h> // for sleep()
#include <vector>
#include "CpuMonitor.h"
#include "ProcessMonitor.h"

int main() {
    CpuMonitor cpu;

    for (int i = 0; i < 5; ++i) {
        cpu.update();

        std::cout << "Total CPU Usage: " << cpu.getCpuUsage() << " %" << std::endl;
        std::cout << "CPU Frequency: " << cpu.getCpuFreq() << " MHz (Max: " << cpu.getCpuFreqMax() << " MHz)" << std::endl;

        std::vector<float> perCore = cpu.getPerCoreUsage();
        for (size_t i = 0; i < perCore.size(); ++i) {
            std::cout << "Core " << i << ": " << perCore[i] << " %" << std::endl;
        }

        std::cout << "----------------------------------\n";
        sleep(1);
    }

    // processMonitor
    ProcessMonitor proc;
    if (proc.update()) {
        std::cout << "\n===== Active Processes =====\n";
        std::cout << "Number of processes: " << proc.getProcessCount() << "\n";
        std::cout << proc.getProcessInfo();
    } else {
        std::cerr << "Failed to update process list.\n";
    }

    return 0;
}