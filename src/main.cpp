#include "../include/CpuMonitor.h"
//#include "CpuMonitor.cpp"
#include <iostream>
#include <unistd.h>

int main() {
    CpuMonitor cpu;

    while (true) {
        cpu.update();

        std::cout << "------------------------------" << std::endl;
        std::cout << "CPU Usage     : " << cpu.getCpuUsage() << " %" << std::endl;
        std::cout << "CPU Frequency : " << cpu.getCpuFreq() << " MHz" << std::endl;
        std::cout << "CPU Info      : " << cpu.getCpuInfo();
        std::cout << "------------------------------" << std::endl;

        sleep(1);
    }

    return 0;
}

