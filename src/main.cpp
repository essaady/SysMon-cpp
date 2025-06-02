

#include "../include/CpuMonitor.h"
#include "CpuMonitor.cpp"
#include <iostream>
#include <unistd.h>  
int main() {
    CpuMonitor cpu;

    while (true) {
        float usage = cpu.getCpuUsage();
        std::cout << "CPU Usage: " << usage << "%" << std::endl;
        sleep(1); 
    }

    return 0;
}
