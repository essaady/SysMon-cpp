#include <iostream>
#include <iomanip>
#include <csignal>
#include <windows.h>
#include <ctime>
#include "../include/ProcessMonitor.h"

// Variable pour gérer l'arrêt du programme
volatile sig_atomic_t running = 1;

// Gestionnaire de signal pour Ctrl+C
void signalHandler(int signum) {
    std::cout << "\nInterruption détectée. Arrêt du programme..." << std::endl;
    running = 0;
}

// Fonction pour effacer l'écran
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";  // Code ANSI pour effacer l'écran
}

int main() {
    // Enregistre le gestionnaire de signal
    signal(SIGINT, signalHandler);
    
    // Crée le moniteur de processus
    ProcessMonitor procMonitor;
    
    std::cout << "SysMon-cpp - Moniteur de processus" << std::endl;
    std::cout << "Appuyez sur Ctrl+C pour quitter" << std::endl;
    
    // Boucle principale
    while (running) {
        // Met à jour les informations des processus
        procMonitor.update();
        
        // Efface l'écran
        clearScreen();
          // Affiche l'en-tête
        std::time_t currentTime = std::time(nullptr);
        std::cout << "SysMon-cpp - Moniteur de processus - " 
                  << std::put_time(std::localtime(&currentTime), "%H:%M:%S") << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        
        // Affiche les 10 processus les plus actifs
        const auto& topProcesses = procMonitor.getTopProcesses(10);
        
        std::cout << std::left << std::setw(8) << "PID" 
                  << std::setw(20) << "NOM" 
                  << std::setw(10) << "CPU%" 
                  << std::setw(10) << "MEM%" 
                  << std::setw(10) << "MEM(kB)" 
                  << std::setw(10) << "ÉTAT" 
                  << "UTILISATEUR" << std::endl;
        
        for (const auto& proc : topProcesses) {
            // Tronque le nom s'il est trop long
            std::string name = proc.name;
            if (name.length() > 19) {
                name = name.substr(0, 16) + "...";
            }
            
            std::cout << std::left << std::setw(8) << proc.pid 
                      << std::setw(20) << name 
                      << std::fixed << std::setprecision(1) << std::setw(10) << proc.cpuUsage 
                      << std::setw(10) << proc.memoryUsage 
                      << std::setw(10) << proc.memoryRss
                      << std::setw(10) << proc.state
                      << proc.user << std::endl;
        }
          // Attente avant la prochaine mise à jour
        Sleep(1000); // Windows API function for sleep in milliseconds
    }
    
    std::cout << "Programme terminé." << std::endl;
    
    return 0;
}