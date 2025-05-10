#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <vector>

// structure forward declaration
struct CpuTime;

/*
class pour surveiller l'utilisation du CPU
 */
class CpuMonitor {
private:
// fct pour lire les statistiques CPU à partir de /proc/stat
    bool readCpuStats(std::vector<CpuTime>& cpuTimes);
    
    // Fonction pour calculer le pourcentage d'utilisation CPU
    double calculateCpuUsage(const CpuTime& prev, const CpuTime& curr);

public:
// Constructeur
    CpuMonitor();
    
// Destructeur
    ~CpuMonitor();
    
// fct principale pour démarrer la surveillance
    void start();
};

#endif // CPU_MONITOR_H