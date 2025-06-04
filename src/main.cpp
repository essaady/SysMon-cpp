#include "../include/CpuMonitor.h"
#include <iostream>
#include <thread>
#include <chrono>
//Hamza Tahiri
int main() {
    CpuMonitor monitor;

    while (true) {
        monitor.update();
        std::cout << "Total CPU Usage: " << monitor.totalUsage() << "%" << std::endl;

        for (const auto& [core, usage] : monitor.coreUsage()) {
            if (core != "cpu")
                std::cout << "  " << core << ": " << usage << "%" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
