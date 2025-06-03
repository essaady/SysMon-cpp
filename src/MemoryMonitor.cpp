#include "../include/MemoryMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

MemoryMonitor::MemoryMonitor() {
    // Initialisation
    update();
}

MemoryMonitor::~MemoryMonitor() {
    // Nettoyage si nécessaire
}

bool MemoryMonitor::update() {
    return readMemInfo();
}

unsigned long long MemoryMonitor::getTotalMemory() const {
    auto it = memInfo.find("MemTotal");
    if (it != memInfo.end()) {
        return it->second;
    }
    return 0;
}

unsigned long long MemoryMonitor::getFreeMemory() const {
    unsigned long long free = 0;
    
    // MemFree + Buffers + Cached
    auto itFree = memInfo.find("MemFree");
    if (itFree != memInfo.end()) {
        free += itFree->second;
    }
    
    auto itBuffers = memInfo.find("Buffers");
    if (itBuffers != memInfo.end()) {
        free += itBuffers->second;
    }
    
    auto itCached = memInfo.find("Cached");
    if (itCached != memInfo.end()) {
        free += itCached->second;
    }
    
    return free;
}

unsigned long long MemoryMonitor::getUsedMemory() const {
    return getTotalMemory() - getFreeMemory();
}

double MemoryMonitor::getMemoryUsagePercentage() const {
    unsigned long long total = getTotalMemory();
    if (total == 0) {
        return 0.0;
    }
    
    return 100.0 * getUsedMemory() / total;
}

unsigned long long MemoryMonitor::getTotalSwap() const {
    auto it = memInfo.find("SwapTotal");
    if (it != memInfo.end()) {
        return it->second;
    }
    return 0;
}

unsigned long long MemoryMonitor::getFreeSwap() const {
    auto it = memInfo.find("SwapFree");
    if (it != memInfo.end()) {
        return it->second;
    }
    return 0;
}

unsigned long long MemoryMonitor::getUsedSwap() const {
    return getTotalSwap() - getFreeSwap();
}

double MemoryMonitor::getSwapUsagePercentage() const {
    unsigned long long total = getTotalSwap();
    if (total == 0) {
        return 0.0;
    }
    
    return 100.0 * getUsedSwap() / total;
}

std::string MemoryMonitor::exportAsText() const {
    std::ostringstream oss;
    
    // Convertir kB en MB pour l'affichage
    const double KB_TO_MB = 1024.0;
    
    oss << "Memory Usage:\n";
    oss << "  Total: " << std::fixed << std::setprecision(2) << getTotalMemory() / KB_TO_MB << " MB\n";
    oss << "  Used: " << std::fixed << std::setprecision(2) << getUsedMemory() / KB_TO_MB << " MB (" 
        << std::fixed << std::setprecision(2) << getMemoryUsagePercentage() << "%)\n";
    oss << "  Free: " << std::fixed << std::setprecision(2) << getFreeMemory() / KB_TO_MB << " MB\n";
    
    oss << "Swap Usage:\n";
    oss << "  Total: " << std::fixed << std::setprecision(2) << getTotalSwap() / KB_TO_MB << " MB\n";
    oss << "  Used: " << std::fixed << std::setprecision(2) << getUsedSwap() / KB_TO_MB << " MB (" 
        << std::fixed << std::setprecision(2) << getSwapUsagePercentage() << "%)\n";
    oss << "  Free: " << std::fixed << std::setprecision(2) << getFreeSwap() / KB_TO_MB << " MB\n";
    
    return oss.str();
}

std::string MemoryMonitor::exportAsCSV() const {
    std::ostringstream oss;
    
    // Convertir kB en MB pour l'affichage
    const double KB_TO_MB = 1024.0;
    
    // En-tête
    oss << "Type,Total(MB),Used(MB),Free(MB),Usage(%)\n";
    
    // Mémoire
    oss << "Memory," 
        << std::fixed << std::setprecision(2) << getTotalMemory() / KB_TO_MB << ","
        << std::fixed << std::setprecision(2) << getUsedMemory() / KB_TO_MB << ","
        << std::fixed << std::setprecision(2) << getFreeMemory() / KB_TO_MB << ","
        << std::fixed << std::setprecision(2) << getMemoryUsagePercentage() << "\n";
    
    // Swap
    oss << "Swap," 
        << std::fixed << std::setprecision(2) << getTotalSwap() / KB_TO_MB << ","
        << std::fixed << std::setprecision(2) << getUsedSwap() / KB_TO_MB << ","
        << std::fixed << std::setprecision(2) << getFreeSwap() / KB_TO_MB << ","
        << std::fixed << std::setprecision(2) << getSwapUsagePercentage() << "\n";
    
    return oss.str();
}

bool MemoryMonitor::readMemInfo() {
    std::ifstream memFile("/proc/meminfo");
    if (!memFile.is_open()) {
        std::cerr << "Failed to open /proc/meminfo" << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(memFile, line)) {
        std::istringstream iss(line);
        std::string key;
        unsigned long long value;
        std::string unit;
        
        // Format: "MemTotal:        8167848 kB"
        if (iss >> key >> value >> unit) {
            // Supprimer le ":" à la fin de la clé
            if (!key.empty() && key.back() == ':') {
                key.pop_back();
            }
            
            memInfo[key] = value;
        }
    }
    
    memFile.close();
    return true;
}