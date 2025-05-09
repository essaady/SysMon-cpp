#include <iostream>
#include "CpuMonitor.h"

int main() {
    CpuMonitor monitor;
    double usage = monitor.getUsage();
    std::cout << "Utilisation du CPU : " << usage << "%" << std::endl;
    return 0;
}
