#include "../include/SysMon.h"
#include <iostream>
#include <string>
#include <csignal>
#include <thread>
#include <chrono>
#include <cstring>

// Variable globale pour gérer l'arrêt propre du programme
volatile sig_atomic_t running = 1;

// Gestionnaire de signal pour capturer Ctrl+C
void signalHandler(int signum) {
    running = 0;
}

void printHelp() {
    std::cout << "SysMon - System Monitor\n";
    std::cout << "Usage: sysmon [options]\n";
    std::cout << "Options:\n";
    std::cout << "  -h, --help            Show this help message\n";
    std::cout << "  -i, --interval <ms>   Set update interval in milliseconds (default: 1000)\n";
    std::cout << "  -n, --processes <n>   Show top n processes (default: 10)\n";
    std::cout << "  -e, --export <file>   Export data to file\n";
    std::cout << "  -f, --format <format> Export format (text, csv) (default: text)\n";
    std::cout << "  -o, --once            Update once and exit\n";
    std::cout << "\nExamples:\n";
    std::cout << "  sysmon                Run with default settings\n";
    std::cout << "  sysmon -i 500 -n 5    Run with 500ms update interval, show top 5 processes\n";
    std::cout << "  sysmon -e data.csv -f csv -o  Export data to CSV file and exit\n";
}

int main(int argc, char* argv[]) {
    // Configurer le gestionnaire de signal
    std::signal(SIGINT, signalHandler);
    
    // Paramètres par défaut
    int updateInterval = 1000;
    int processCount = 10;
    std::string exportFile = "";
    ExportFormat exportFormat = ExportFormat::TEXT;
    bool updateOnce = false;
    
    // Analyser les arguments de ligne de commande
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printHelp();
            return 0;
        } else if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--interval") == 0) {
            if (i + 1 < argc) {
                updateInterval = std::stoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "--processes") == 0) {
            if (i + 1 < argc) {
                processCount = std::stoi(argv[++i]);
            }
        } else if (strcmp(argv[i], "-e") == 0 || strcmp(argv[i], "--export") == 0) {
            if (i + 1 < argc) {
                exportFile = argv[++i];
            }
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--format") == 0) {
            if (i + 1 < argc) {
                std::string format = argv[++i];
                if (format == "csv") {
                    exportFormat = ExportFormat::CSV;
                } else {
                    exportFormat = ExportFormat::TEXT;
                }
            }
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--once") == 0) {
            updateOnce = true;
        }
    }
    
    // Créer l'instance de SysMon
    SysMon sysMon(updateInterval);
    
    // Mode d'exportation unique
    if (!exportFile.empty() && updateOnce) {
        sysMon.update();
        if (sysMon.exportData(exportFile, exportFormat, processCount)) {
            std::cout << "Data exported to " << exportFile << std::endl;
        } else {
            std::cerr << "Failed to export data" << std::endl;
            return 1;
        }
        return 0;
    }
    
    // Mode d'affichage unique
    if (updateOnce) {
        sysMon.update();
        sysMon.display(processCount);
        return 0;
    }
    
    // Mode interactif
    std::cout << "SysMon - System Monitor (Press Ctrl+C to exit)\n";
    
    while (running) {
        // Effacer l'écran
        std::cout << "\033[2J\033[1;1H";
        
        // Mettre à jour et afficher les informations
        sysMon.update();
        sysMon.display(processCount);
        
        // Exporter si demandé
        if (!exportFile.empty()) {
            sysMon.exportData(exportFile, exportFormat, processCount);
        }
        
        // Attendre l'intervalle de mise à jour
        std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
    }
    
    std::cout << "\nExiting SysMon...\n";
    
    return 0;
}
