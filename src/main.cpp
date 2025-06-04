#include <iostream>
#include <thread>
#include <chrono>
#include "CpuMonitor.h"

int main() {
    CpuMonitor monitor;

    while (true) {
        if (!monitor.update()) {
            std::cerr << "Erreur mise à jour CPU\n";
            return 1;
        }

        std::cout << monitor.getCpuInfo() << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
