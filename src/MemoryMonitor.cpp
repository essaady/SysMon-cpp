#include "MemoryMonitor.h"
//Constructeur de la classe
MemoryMonitor::MemoryMonitor() {
    updateMemoryUsage();
}

//Méthode pour mettre a jour les valeurs de la mémoire
void MemoryMonitor::updateMemoryUsage() {
    parseMemInfo();
}
//Méthode pour récupérer les informations memoire
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
//fermer le fichier après lecture
    totalMemory = memTotal / 1024;  // Convertir en MB

    usedMemory = (memTotal - memAvailable) / 1024;
}

int MemoryMonitor::getTotalMemory() {
    return totalMemory;
}

int MemoryMonitor::getUsedMemory() {
    return usedMemory;
}
//Fonction "getCpuCores()" pour obtenir les c?urs CPU avec une valeur par d?faut en cas d'erreur.  
void MemoryMonitor::readProcFile(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Erreur : Impossible d'ouvrir " + filePath);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
}























