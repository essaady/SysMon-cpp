#include "CpuMonitor.h"
#include <iostream>

void testCpuUsage()
{
    CpuMonitor cpuMon;
    cpuMon.update();
    std::cout << "CPU Usage: " << cpuMon.getCpuUsage() << "%" << std::endl;
}

int main()
{
    testCpuUsage();
    return 0;
}
