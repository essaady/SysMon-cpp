#include <iostream>
#include <thread>
#include <chrono>
#include "../include/CpuMonitor.h"

int main() {
    CpuMonitor cpuMon;

    while (true) {
        cpuMon.update();
        std::cout << cpuMon.getCpuInfo() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
