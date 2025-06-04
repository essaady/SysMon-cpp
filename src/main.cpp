#include <iostream>
#include "CpuMonitor.h"

int main() {
    CpuMonitor monitor;
    monitor.update();

    std::cout << monitor.getCpuInfo() << std::endl;
    std::cout << "Usage CPU global : " << monitor.getCpuUsage() << "%" << std::endl;
    std::cout << "Fréquence actuelle : " << monitor.getCpuFreq() << " GHz" << std::endl;

    return 0;
}
