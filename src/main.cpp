#include <iostream>
#include "CpuMonitor.h"

int main() {
    CpuMonitor monitor;
    std::cout << "CPU usage: " << monitor.getUsage() << "%" << std::endl;
    return 0;
}
