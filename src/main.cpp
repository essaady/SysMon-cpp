#include <iostream>
#include "CpuMonitor.h"
#include <thread>
#include <chrono>

int main() {
    CpuMonitor monitor;

    while (true) {
        double usage = monitor.getCpuUsage();
        if (usage < 0) {
            std::cerr << "Erreur lecture CPU\n";
            return 1;
        }

        std::cout << "Utilisation CPU : " << usage << " %" << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}
