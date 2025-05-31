// main.cpp
#include <iostream>
#include <string>
#include <fstream>
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"

int main(int argc, char* argv[]) {
    std::string exportFile = "";

    // Analyse des arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--version") {
            std::cout << "SysMon-cpp version 1.0" << std::endl;
            return 0;
        }

        if (arg == "--help") {
            std::cout << "Options disponibles :" << std::endl;
            std::cout << "  --version            Affiche la version du logiciel" << std::endl;
            std::cout << "  --help               Affiche ce message d'aide" << std::endl;
            std::cout << "  --export=nom.csv     Exporte les donn\xE9es syst\xE8me dans un fichier CSV" << std::endl;
            return 0;
        }

        if (arg.find("--export=") == 0) {
            exportFile = arg.substr(9); // R\xE9cup\xE8re le nom de fichier
        }
    }

    // Si l'option --export est utilis\xE9e
    if (!exportFile.empty()) {
        CpuMonitor cpu;
        MemoryMonitor mem;

        cpu.update();
        mem.update();

        std::ofstream file(exportFile);
        if (file.is_open()) {
            file << "CPU Usage (%),Memory Used (MB),Memory Total (MB)\n";
            file << cpu.getUsage() << "," << mem.getUsedMemory() << "," << mem.getTotalMemory() << "\n";
            file.close();
            std::cout << "\u2705 Donn\xE9es export\xE9es dans " << exportFile << std::endl;
        } else {
            std::cerr << "\u274C Impossible d'ouvrir le fichier : " << exportFile << std::endl;
        }
        return 0;
    }

    std::cout << "Bienvenue dans SysMon-cpp !" << std::endl;
    std::cout << "Utilisez --help pour voir les options disponibles." << std::endl;

    // TODO: Ajouter boucle d'affichage temps r\xE9el ici si besoin

    return 0;
}
