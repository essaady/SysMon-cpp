#include "../include/CpuMonitor.h"
#include <iostream>
#include <cassert>

int main() {
    CpuMonitor monitor;
    double usage = monitor.getCpuUsage();

    std::cout << "Test CPU Usage: " << usage << "%" << std::endl;
    assert(usage >= 0.0 && usage <= 100.0);
    std::cout << "✅ Test Passed: usage is in valid range." << std::endl;
    return 0;
}
