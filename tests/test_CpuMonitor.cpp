#include "include/CpuMonitor.h"
#include <iostream>

int main() {
    CPUMonitor monitor;
    for (int i = 0; i < 5; ++i) {
        std::cout << monitor.getFormattedUsage() << std::endl;
    }
    return 0;
}
