#include "MemoryMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

// Constructeur
MemoryMonitor::MemoryMonitor() {
    // Initialisation - on peut faire une première lecture
    update();
}

// Destructeur
MemoryMonitor::~MemoryMonitor() {
    // Nettoyage si nécessaire
    memInfo.clear();
}

// Met à jour les données d'utilisation de la mémoire
bool MemoryMonitor::update() {
    return readMemInfo();
}

// Lit les informations depuis /proc/meminfo
bool MemoryMonitor::readMemInfo() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir /proc/meminfo" << std::endl;
        return false;
    }
    
    memInfo.clear();
    std::string line;
    
    // Lire toutes les lignes du fichier
    while (std::getline(file, line)) {
        memInfo.push_back(line);
    }
    
    file.close();
    return true;
}

// Parse une valeur depuis une ligne de /proc/meminfo
unsigned long long MemoryMonitor::parseMemValue(const std::string& line) {
    std::istringstream iss(line);
    std::string label;
    unsigned long long value;
    std::string unit;
    
    iss >> label >> value >> unit;
    
    // Convertir en bytes si l'unité est kB
    if (unit == "kB") {
        value *= 1024;
    }
    
    return value;
}

// Convertit bytes en MB
double MemoryMonitor::bytesToMB(unsigned long long bytes) {
    return static_cast<double>(bytes) / (1024.0 * 1024.0);
}

// Renvoie la mémoire physique totale
unsigned long long MemoryMonitor::getTotalMemory() {
    for (const std::string& line : memInfo) {
        if (line.find("MemTotal:") == 0) {
            return parseMemValue(line);
        }
    }
    return 0;
}

// Renvoie la mémoire physique libre
unsigned long long MemoryMonitor::getFreeMemory() {
    unsigned long long memFree = 0;
    unsigned long long buffers = 0;
    unsigned long long cached = 0;
    
    for (const std::string& line : memInfo) {
        if (line.find("MemFree:") == 0) {
            memFree = parseMemValue(line);
        } else if (line.find("Buffers:") == 0) {
            buffers = parseMemValue(line);
        } else if (line.find("Cached:") == 0) {
            cached = parseMemValue(line);
        }
    }
    
    // La mémoire réellement disponible inclut les buffers et cache
    return memFree + buffers + cached;
}

// Renvoie la mémoire d'échange totale
unsigned long long MemoryMonitor::getTotalSwap() {
    for (const std::string& line : memInfo) {
        if (line.find("SwapTotal:") == 0) {
            return parseMemValue(line);
        }
    }
    return 0;
}

// Renvoie la mémoire d'échange utilisée
unsigned long long MemoryMonitor::getUsedSwap() {
    unsigned long long swapTotal = getTotalSwap();
    unsigned long long swapFree = 0;
    
    for (const std::string& line : memInfo) {
        if (line.find("SwapFree:") == 0) {
            swapFree = parseMemValue(line);
            break;
        }
    }
    
    return swapTotal - swapFree;
}

// Renvoie le pourcentage de la RAM actuellement utilisée
double MemoryMonitor::getMemoryUsagePercentage() {
    unsigned long long totalMem = getTotalMemory();
    unsigned long long freeMem = getFreeMemory();
    
    if (totalMem == 0) return 0.0;
    
    unsigned long long usedMem = totalMem - freeMem;
    return (static_cast<double>(usedMem) / static_cast<double>(totalMem)) * 100.0;
}

// Renvoie le pourcentage de la mémoire d'échange actuellement utilisée
double MemoryMonitor::getSwapUsagePercentage() {
    unsigned long long totalSwap = getTotalSwap();
    unsigned long long usedSwap = getUsedSwap();
    
    if (totalSwap == 0) return 0.0;
    
    return (static_cast<double>(usedSwap) / static_cast<double>(totalSwap)) * 100.0;
}

// Renvoie les informations formatées de la mémoire
std::string MemoryMonitor::getMemoryInfo() {
    std::ostringstream info;
    
    double totalMemMB = bytesToMB(getTotalMemory());
    double freeMemMB = bytesToMB(getFreeMemory());
    double usedMemMB = totalMemMB - freeMemMB;
    double memUsagePercent = getMemoryUsagePercentage();
    
    double totalSwapMB = bytesToMB(getTotalSwap());
    double usedSwapMB = bytesToMB(getUsedSwap());
    double swapUsagePercent = getSwapUsagePercentage();
    
    info << std::fixed << std::setprecision(2);
    info << "=== INFORMATIONS MÉMOIRE ===" << std::endl;
    info << "RAM Totale: " << totalMemMB << " MB" << std::endl;
    info << "RAM Utilisée: " << usedMemMB << " MB" << std::endl;
    info << "RAM Libre: " << freeMemMB << " MB" << std::endl;
    info << "Utilisation RAM: " << memUsagePercent << "%" << std::endl;
    info << std::endl;
    info << "SWAP Total: " << totalSwapMB << " MB" << std::endl;
    info << "SWAP Utilisé: " << usedSwapMB << " MB" << std::endl;
    info << "Utilisation SWAP: " << swapUsagePercent << "%" << std::endl;
    
    return info.str();
}

// Renvoie la structure RAM complète
RAM MemoryMonitor::getRAMStruct() {
    RAM ram;
    
    // Remplir la structure avec les données actuelles
    unsigned long long totalMem = getTotalMemory();
    unsigned long long freeMem = getFreeMemory();
    unsigned long long totalSwap = getTotalSwap();
    unsigned long long freeSwap = totalSwap - getUsedSwap();
    
    ram.totalMemInMb = static_cast<size_t>(bytesToMB(totalMem));
    ram.freeMem = static_cast<float>(bytesToMB(freeMem));
    ram.swapMemInMb = static_cast<size_t>(bytesToMB(totalSwap));
    ram.freeSwap = static_cast<float>(bytesToMB(freeSwap));
    ram.usage = static_cast<float>(getMemoryUsagePercentage());
    ram.usageSwap = static_cast<float>(getSwapUsagePercentage());
    
    return ram;
}