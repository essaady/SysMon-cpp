#include "SysMon.h"
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <iostream>

int main(int argc, char *argv[])
{
    // Initialisation des modules
    CpuMonitor cpuMon;
    MemoryMonitor memMon;
    ProcessMonitor procMon;
    SysMon sysMon;

    // Mise à jour des données système
    if (!sysMon.update(cpuMon, memMon, procMon))
    {
        std::cerr << "Erreur : Impossible de mettre à jour les données système." << std::endl;
        return 1;
    }

    // Affichage des informations système
    std::cout << "===== SysMon-cpp =====" << std::endl;
    std::cout << "Heure : " << sysMon.getTime() << std::endl;
    std::cout << "Informations Système : " << sysMon.getInfo() << std::endl;
    std::cout << "Utilisation du CPU : " << cpuMon.getCpuUsage() << "%" << std::endl;
    std::cout << "Fréquence du CPU : " << cpuMon.getCpuFreq() << " GHz" << std::endl;
    std::cout << "Mémoire totale : " << memMon.getTotalMemory() << " Mo" << std::endl;
    std::cout << "Mémoire libre : " << memMon.getFreeMemory() << " Mo" << std::endl;
    std::cout << "Liste des processus actifs : " << std::endl;
    std::cout << procMon.getProcessInfo() << std::endl;

    return 0;
}
