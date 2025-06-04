#include "SysMon.h"
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
std::string SysMon::exportAsText()
{
    std::ostringstream textOutput;

    textOutput << "===== SysMon System Report =====\n";
    textOutput << "Heure: " << getTime() << "\n";
    textOutput << "Informations Système: " << getInfo() << "\n";
    textOutput << "------------------------------\n";
    textOutput << "CPU Usage: " << cpuMonitor.getCpuUsage() << "%\n";
    textOutput << "CPU Frequency: " << cpuMonitor.getCpuFreq() << " GHz\n";
    textOutput << "------------------------------\n";
    textOutput << "Mémoire totale: " << memoryMonitor.getTotalMemory() << " Mo\n";
    textOutput << "Mémoire libre: " << memoryMonitor.getFreeMemory() << " Mo\n";
    textOutput << "------------------------------\n";
    textOutput << "Processus actifs:\n"
               << processMonitor.getProcessInfo() << "\n";

    return textOutput.str();
}

std::string SysMon::exportAsCSV()
{
    std::ostringstream csvOutput;

    // En-tête CSV
    csvOutput << "Heure,Infos Système,CPU Usage,CPU Frequency,Total Mémoire,Libre Mémoire,Processus\n";

    // Données système
    csvOutput << getTime() << ",";
    csvOutput << getInfo() << ",";
    csvOutput << cpuMonitor.getCpuUsage() << ",";
    csvOutput << cpuMonitor.getCpuFreq() << ",";
    csvOutput << memoryMonitor.getTotalMemory() << ",";
    csvOutput << memoryMonitor.getFreeMemory() << ",";
    csvOutput << "\"" << processMonitor.getProcessInfo() << "\"\n";

    return csvOutput.str();
}

int SysMon::run()
{
    std::cout << "Démarrage de SysMon...\n";

    while (true)
    { // Boucle infinie pour la surveillance continue
        CpuMonitor cpuMon;
        MemoryMonitor memMon;
        ProcessMonitor procMon;

        cpuMon.update();
        memMon.update();
        procMon.update();

        std::cout << "Utilisation CPU: " << cpuMon.getCpuUsage() << "%\n";
        std::cout << "Mémoire libre: " << memMon.getFreeMemory() << " Mo\n";
        std::cout << "Processus actifs:\n"
                  << procMon.getProcessInfo() << "\n";

        std::this_thread::sleep_for(std::chrono::seconds(1)); // Pause de 1 seconde avant la prochaine mise à jour
    }

    return 0;
}

bool SysMon::update(CpuMonitor cpuMon, MemoryMonitor memMon, ProcessMonitor procMon)
{
    return cpuMon.update() && memMon.update() && procMon.update();
}

std::string SysMon::getTime()
{
    return "12:00 PM"; // Exemple d'heure système
}

std::string SysMon::getInfo()
{
    return "SysMon - Moniteur de ressources système en C++.";
}
