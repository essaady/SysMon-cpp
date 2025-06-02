#include "MemoryMonitor.h"
#include <iostream>
#include <sstream>
#include <iomanip>

MemoryMonitor::MemoryMonitor() : memInfoPath("/proc/meminfo") {
    // Constructeur
}

MemoryMonitor::~MemoryMonitor() {
    // Destructeur
}

bool MemoryMonitor::readMemInfo() {
    std::ifstream file(memInfoPath);
    if (!file.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir " << memInfoPath << std::endl;
        return false;
    }
    
    memoryData.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            unsigned long value = parseMemoryValue(line);
            memoryData[key] = value;
        }
    }
    
    file.close();
    return true;
}

unsigned long MemoryMonitor::parseMemoryValue(const std::string& line) {
    std::istringstream iss(line);
    std::string key, unit;
    unsigned long value;
    
    iss >> key >> value >> unit;
    
    // La plupart des valeurs dans /proc/meminfo sont en kB
    return value;
}

void MemoryMonitor::calculateUsage(MemoryInfo& info) {
    // Calcul de la mémoire utilisée
    info.usedMemory = info.totalMemory - info.freeMemory - info.buffers - info.cached;
    
    // Calcul des pourcentages
    if (info.totalMemory > 0) {
        info.memoryUsagePercent = (static_cast<double>(info.usedMemory) / info.totalMemory) * 100.0;
    } else {
        info.memoryUsagePercent = 0.0;
    }
    
    if (info.totalSwap > 0) {
        info.swapUsagePercent = (static_cast<double>(info.usedSwap) / info.totalSwap) * 100.0;
    } else {
        info.swapUsagePercent = 0.0;
    }
}

bool MemoryMonitor::getMemoryInfo(MemoryInfo& info) {
    if (!readMemInfo()) {
        return false;
    }
    
    // Extraction des valeurs importantes
    info.totalMemory = memoryData["MemTotal"];
    info.freeMemory = memoryData["MemFree"];
    info.availableMemory = memoryData["MemAvailable"];
    info.buffers = memoryData["Buffers"];
    info.cached = memoryData["Cached"];
    info.totalSwap = memoryData["SwapTotal"];
    info.freeSwap = memoryData["SwapFree"];
    info.usedSwap = info.totalSwap - info.freeSwap;
    
    // Calculs
    calculateUsage(info);
    
    return true;
}

void MemoryMonitor::displayMemoryInfo() {
    MemoryInfo info;
    if (getMemoryInfo(info)) {
        displayMemoryInfo(info);
    } else {
        std::cerr << "Erreur lors de la récupération des informations mémoire" << std::endl;
    }
}

void MemoryMonitor::displayMemoryInfo(const MemoryInfo& info) {
    std::cout << "\n=== INFORMATIONS MÉMOIRE ===" << std::endl;
    
    // Mémoire RAM
    std::cout << "Mémoire RAM:" << std::endl;
    std::cout << "  Total:      " << formatBytes(info.totalMemory * 1024) << std::endl;
    std::cout << "  Utilisée:   " << formatBytes(info.usedMemory * 1024) 
              << " (" << std::fixed << std::setprecision(1) << info.memoryUsagePercent << "%)" << std::endl;
    std::cout << "  Libre:      " << formatBytes(info.freeMemory * 1024) << std::endl;
    std::cout << "  Disponible: " << formatBytes(info.availableMemory * 1024) << std::endl;
    std::cout << "  Buffers:    " << formatBytes(info.buffers * 1024) << std::endl;
    std::cout << "  Cache:      " << formatBytes(info.cached * 1024) << std::endl;
    
    // Barre de progression pour la RAM
    std::cout << "  [";
    int barWidth = 30;
    int progress = static_cast<int>((info.memoryUsagePercent / 100.0) * barWidth);
    for (int i = 0; i < barWidth; ++i) {
        if (i < progress) {
            std::cout << "█";
        } else {
            std::cout << "░";
        }
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << info.memoryUsagePercent << "%" << std::endl;
    
    // Mémoire Swap
    if (info.totalSwap > 0) {
        std::cout << "\nMémoire Swap:" << std::endl;
        std::cout << "  Total:    " << formatBytes(info.totalSwap * 1024) << std::endl;
        std::cout << "  Utilisée: " << formatBytes(info.usedSwap * 1024) 
                  << " (" << std::fixed << std::setprecision(1) << info.swapUsagePercent << "%)" << std::endl;
        std::cout << "  Libre:    " << formatBytes(info.freeSwap * 1024) << std::endl;
        
        // Barre de progression pour le Swap
        std::cout << "  [";
        int swapProgress = static_cast<int>((info.swapUsagePercent / 100.0) * barWidth);
        for (int i = 0; i < barWidth; ++i) {
            if (i < swapProgress) {
                std::cout << "█";
            } else {
                std::cout << "░";
            }
        }
        std::cout << "] " << std::fixed << std::setprecision(1) << info.swapUsagePercent << "%" << std::endl;
    } else {
        std::cout << "\nMémoire Swap: Non configurée" << std::endl;
    }
}

std::string MemoryMonitor::formatBytes(unsigned long bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
    return oss.str();
}

bool MemoryMonitor::isMemoryAvailable() {
    std::ifstream file(memInfoPath);
    return file.is_open();
}