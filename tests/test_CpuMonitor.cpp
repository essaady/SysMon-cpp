#include "../include/CpuMonitor.h"
#include <iostream>
#include <unistd.h>
#include <iomanip>

int main() {
    std::cout << "=== TEST DU MODULE CPU ===" << std::endl;
    
    // Créer une instance du moniteur CPU
    CpuMonitor cpuMonitor;
    
    // Afficher les infos du CPU
    std::cout << "CPU Info: " << cpuMonitor.getCpuInfo() << std::endl;
    std::cout << std::endl;
    
    // Surveiller pendant quelques secondes
    std::cout << "Surveillance du CPU pendant 10 secondes..." << std::endl;
    std::cout << "Appuyez sur Ctrl+C pour arrêter." << std::endl;
    std::cout << std::endl;
    
    for (int i = 0; i < 10; i++) {
        cpuMonitor.displayCpuUsage();
        sleep(1); // Attendre 1 seconde
    }
    
    std::cout << "Test terminé !" << std::endl;
    return 0;
}