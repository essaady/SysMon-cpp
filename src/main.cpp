#include "../include/SysMon.h"
#include <iostream>
#include <string>
#include <csignal>
#include <thread>
#include <chrono>
#include <cstring>

volatile sig_atomic_t isRunning = 1;

void handleSignal(int) {
    isRunning = 0;
}

void showUsage() {
    std::cout << "SysMon - System Monitor\n";
    std::cout << "Usage: sysmon [options]\n\n";
    std::cout << "Options disponibles:\n";
    std::cout << "  -h, --help              Affiche ce message d'aide\n";
    std::cout << "  -i, --interval <ms>     Intervalle de mise à jour (par défaut: 1000 ms)\n";
    std::cout << "  -n, --processes <n>     Nombre de processus à afficher (par défaut: 10)\n";
    std::cout << "  -e, --export <fichier>  Exporter les données vers un fichier\n";
    std::cout << "  -f, --format <format>   Format d'exportation (text, csv). Par défaut: text\n";
    std::cout << "  -o, --once              Exécuter une seule mise à jour puis quitter\n\n";
    std::cout << "Exemples:\n";
    std::cout << "  sysmon\n";
    std::cout << "  sysmon -i 500 -n 5\n";
    std::cout << "  sysmon -e log.csv -f csv -o\n";
}

int main(int argc, char* argv[]) {
    std::signal(SIGINT, handleSignal);

    int intervalMs = 1000;
    int topProcesses = 10;
    std::string outputPath = "";
    ExportFormat outputFormat = ExportFormat::TEXT;
    bool singleUpdate = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            showUsage();
            return 0;
        } else if ((arg == "-i" || arg == "--interval") && i + 1 < argc) {
            intervalMs = std::stoi(argv[++i]);
        } else if ((arg == "-n" || arg == "--processes") && i + 1 < argc) {
            topProcesses = std::stoi(argv[++i]);
        } else if ((arg == "-e" || arg == "--export") && i + 1 < argc) {
            outputPath = argv[++i];
        } else if ((arg == "-f" || arg == "--format") && i + 1 < argc) {
            std::string fmt = argv[++i];
            outputFormat = (fmt == "csv") ? ExportFormat::CSV : ExportFormat::TEXT;
        } else if (arg == "-o" || arg == "--once") {
            singleUpdate = true;
        }
    }

    SysMon monitor(intervalMs);

    if (!outputPath.empty() && singleUpdate) {
        monitor.update();
        if (monitor.exportData(outputPath, outputFormat, topProcesses)) {
            std::cout << "Export réussi : " << outputPath << "\n";
        } else {
            std::cerr << "Échec de l'exportation\n";
            return 1;
        }
        return 0;
    }

    if (singleUpdate) {
        monitor.update();
        monitor.display(topProcesses);
        return 0;
    }

    std::cout << "SysMon - Surveillance système (Ctrl+C pour quitter)\n";

    while (isRunning) {
        std::cout << "\033[2J\033[1;1H"; 
        monitor.update();
        monitor.display(topProcesses);

        if (!outputPath.empty()) {
            monitor.exportData(outputPath, outputFormat, topProcesses);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }

    std::cout << "\nArrêt de SysMon...\n";
    return 0;
}
