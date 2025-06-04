#include <iostream>
#include <sstream> // permet de traiter les chaînes comme des flux, ce qui est utile pour l'insertion, l'extraction et la conversion de données. 
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include "SysMon.h"

// Implémentation de la classe SysMon dans main
void SysMon::updateAll() {
    cpuMonitor.update();
    memoryMonitor.update();
    processMonitor.update();
}

std::string SysMon::getReport() const {
    std::ostringstream oss;
    oss << "=== Rapport SysMon ===\n";
    oss << "CPU Usage: " << cpuMonitor.getCpuUsage() << "%\n";
    oss << "CPU Info: " << cpuMonitor.getCpuInfo() << "\n";
    oss << "Memory Usage: " << memoryMonitor.getMemoryUsage() << "%\n";
    oss << "Memory Info: " << memoryMonitor.getMemoryInfo() << "\n\n";
    oss << "Processus actifs :\n" << processMonitor.getProcessInfo() << "\n";
    return oss.str();
}

int main() {
    SysMon systemMonitor;

    // Mise à jour des infos système
    systemMonitor.updateAll();

    // Affichage du rapport complet
    std::cout << systemMonitor.getReport() << std::endl;

    try {
        ProcessMonitor pm;
        pm.update();
        AP proc = pm.getProcess(0); // Premier processus

        std::cout << "Détails Processus 0 : PID=" << proc.pid
                  << ", User=" << proc.user
                  << ", CPU=" << proc.cpu << "%"
                  << ", Mémoire=" << proc.memory << "MB"
                  << ", Path=" << proc.pathName
                  << ", Start=" << proc.time.startTime
                  << ", RunTime=" << proc.time.runTime << std::endl;

    } catch (const std::out_of_range& e) {
        std::cerr << "Erreur d'accès au processus : " << e.what() << std::endl;
    }

    return 0;
}
