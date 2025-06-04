// src/main.cpp
#include <iostream>
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <string>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];

        if (arg == "--version") {
            std::cout << "SysMon-cpp v1.0.0" << std::endl;
            return 0;
        } else if (arg == "--help") {
            std::cout << "Options disponibles :\n"
                      << "  --version           Affiche la version\n"
                      << "  --help              Affiche cette aide\n"
                      << "  --export            Exporte les données\n"
                      << "  --update-interval N Définit l’intervalle de mise à jour (en secondes)\n";
            return 0;
        }
    }
   CpuMonitor cpu;
    MemoryMonitor mem;
    ProcessMonitor proc;

    cpu.update();
    mem.update();
    proc.update();

    std::cout << "=== Moniteur Système ===" << std::endl;
    std::cout << "CPU Usage: " << cpu.getCpuUsage() << "%" << std::endl;
    std::cout << "CPU Frequency: " << cpu.getCpuFreq() << " MHz" << std::endl;

    std::cout << "RAM Usage: " << mem.getMemoryUsagePercentage() << "%" << std::endl;
    std::cout << "Swap Usage: " << mem.getSwapUsagePercentage() << "%" << std::endl;

    std::cout << proc.getProcessInfo() << std::endl;
  return 0;

}


