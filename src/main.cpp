#include "../include/CpuMonitor.h"
#include <iostream>

int main() {
    CpuMonitor monitor;

    while (true) {
        double usage = monitor.getCpuUsage();
        std::cout << "CPU Usage: " << usage << "%" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
