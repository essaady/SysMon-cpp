#include "../include/CpuMonitor.h"
#include <iostream>
#include <windows.h>

/**
 * @brief Programme de test pour la surveillance CPU
 * 
 * Ce programme démontre l'utilisation de la classe CpuMonitor
 * en affichant l'utilisation CPU toutes les secondes.
 */
int main() {
    // Créer une instance du moniteur CPU
    CpuMonitor cpuMonitor;
    
    std::cout << "Démarrage de la surveillance CPU... (Ctrl+C pour quitter)" << std::endl;
    
    // Boucle principale : affiche l'utilisation CPU chaque seconde
    while (true) {
        std::cout << cpuMonitor.getFormattedUsage() << std::endl;
        Sleep(1000);  // Sleep for 1000 milliseconds (1 second)
    }
    
    return 0;
} 