#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <vector>
#include <string>

struct RAM {
    size_t totalMemInMb;    // Mémoire physique totale en mégaoctets
    float freeMem;          // Mémoire physique libre
    size_t swapMemInMb;     // Mémoire d'échange (swap) totale en mégaoctets
    float freeSwap;         // Mémoire d'échange libre
    float usage;            // Pourcentage d'utilisation de la RAM
    float usageSwap;        // Pourcentage d'utilisation du swap
};

class MemoryMonitor {
private:
    std::vector<std::string> memInfo;  // Stockage des informations mémoire brutes

public:
    // Constructeur
    MemoryMonitor();
    
    // Destructeur
    ~MemoryMonitor();
    
    // Méthodes principales
    bool update();                           // Met à jour les données d'utilisation de la mémoire et de la mémoire d'échange
    unsigned long long getTotalMemory();     // Renvoie la mémoire physique totale
    unsigned long long getFreeMemory();      // Renvoie la mémoire physique libre
    unsigned long long getTotalSwap();       // Renvoie la mémoire d'échange totale
    unsigned long long getUsedSwap();        // Renvoie la mémoire d'échange utilisée
    double getMemoryUsagePercentage();       // Renvoie le pourcentage de la RAM actuellement utilisée
    double getSwapUsagePercentage();         // Renvoie le pourcentage de la mémoire d'échange actuellement utilisée
    
    // Méthodes utilitaires
    std::string getMemoryInfo();             // Renvoie les informations formatées de la mémoire
    RAM getRAMStruct();                      // Renvoie la structure RAM complète
    
private:
    // Méthodes privées d'aide
    bool readMemInfo();                      // Lit les informations depuis /proc/meminfo
    unsigned long long parseMemValue(const std::string& line);  // Parse une valeur depuis une ligne de /proc/meminfo
    double bytesToMB(unsigned long long bytes);  // Convertit bytes en MB
};

#endif // MEMORYMONITOR_H