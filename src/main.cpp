#include <iostream>
#include <unistd.h>
#include "CpuMonitor.h"

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

    return 0;
}
