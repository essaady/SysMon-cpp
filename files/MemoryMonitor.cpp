#include "MemoryMonitor.h"
//Constructeur de la classe
MemoryMonitor::MemoryMonitor() {
    updateMemoryUsage();
}

//M�thode pour mettre a jour les valeurs de la m�moire
void MemoryMonitor::updateMemoryUsage() {
    parseMemInfo();
}
//M�thode pour r�cup�rer les informations memoire
void MemoryMonitor::parseMemInfo() {
    std::ifstream memFile("/proc/meminfo");
    std::string line;
    int memTotal = 0, memAvailable = 0;

    while (std::getline(memFile, line)) {
        if (line.find("MemTotal") != std::string::npos) {
            sscanf(line.c_str(), "MemTotal: %d kB", &memTotal);
        } else if (line.find("MemAvailable") != std::string::npos) {
            sscanf(line.c_str(), "MemAvailable: %d kB", &memAvailable);
        }
    }
    memFile.close();
//fermer le fichier apr�s lecture
    totalMemory = memTotal / 1024;  // Convertir en MB

    usedMemory = (memTotal - memAvailable) / 1024;
}

int MemoryMonitor::getTotalMemory() {
    return totalMemory;
}

int MemoryMonitor::getUsedMemory() {
    return usedMemory;
}
























