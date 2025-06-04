 #include "CpuMonitor.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    CpuMonitor cpu;

    while (true) {
        cpu.update();
        std::cout << cpu.getCpuInfo() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}

 
