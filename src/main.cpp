#include <iostream>
#include <thread>
#include <chrono>
#include "../include/CpuMonitor.h"
#include "../include/MemoryMonitor.h"

int main() {
    CpuMonitor cpuMonitor;
    MemoryMonitor memMonitor;

    while (true) {
        cpuMonitor.update();
        memMonitor.update();

        std::cout << cpuMonitor.exportAsText() << std::endl;
        std::cout << memMonitor.exportAsText() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "-------------------------------------" << std::endl;
    }

    return 0;
}
