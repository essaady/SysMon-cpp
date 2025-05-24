#include <iostream>
#include <iomanip>
#include "../include/ProcessMonitor.h"

int main() {
    // Création du moniteur de processus
    ProcessMonitor monitor;
    
    std::cout << "=== Test du moniteur de processus ===" << std::endl;
    
    // Mise à jour des informations
    monitor.update();
    
    // Affichage des processus les plus actifs
    const auto& topProcesses = monitor.getTopProcesses(5);
    
    std::cout << "Top 5 processus par utilisation CPU:" << std::endl;
    std::cout << std::left << std::setw(8) << "PID" 
              << std::setw(20) << "NOM" 
              << std::setw(8) << "CPU%" 
              << std::setw(8) << "MEM%" 
              << "UTILISATEUR" << std::endl;
    
    for (const auto& proc : topProcesses) {
        std::cout << std::left << std::setw(8) << proc.pid 
                  << std::setw(20) << proc.name 
                  << std::fixed << std::setprecision(1) << std::setw(8) << proc.cpuUsage 
                  << std::setw(8) << proc.memoryUsage 
                  << proc.user << std::endl;
    }
    
    // Attendre quelques secondes pour voir la différence
    std::cout << "\nAttente de 2 secondes..." << std::endl;
    sleep(2);
    
    // Mise à jour et affichage à nouveau
    monitor.update();
    
    const auto& updatedProcesses = monitor.getTopProcesses(5);
    
    std::cout << "\nTop 5 processus après mise à jour:" << std::endl;
    std::cout << std::left << std::setw(8) << "PID" 
              << std::setw(20) << "NOM" 
              << std::setw(8) << "CPU%" 
              << std::setw(8) << "MEM%" 
              << "UTILISATEUR" << std::endl;
    
    for (const auto& proc : updatedProcesses) {
        std::cout << std::left << std::setw(8) << proc.pid 
                  << std::setw(20) << proc.name 
                  << std::fixed << std::setprecision(1) << std::setw(8) << proc.cpuUsage 
                  << std::setw(8) << proc.memoryUsage 
                  << proc.user << std::endl;
    }
    
    std::cout << "\nTest terminé avec succès!" << std::endl;
    
    return 0;
}