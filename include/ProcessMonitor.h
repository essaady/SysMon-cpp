#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <vector>
#include <string>
#include <map>

// Structure pour stocker les informations d'un processus
struct ProcessInfo {
    int pid;                  // ID du processus
    std::string name;         // Nom du processus
    std::string state;        // État du processus (R, S, D, Z, etc.)
    double cpuUsage;          // Utilisation CPU en pourcentage
    double memoryUsage;       // Utilisation mémoire en pourcentage
    long memoryRss;           // Mémoire résidente (RSS) en kB
    std::string user;         // Utilisateur propriétaire
};

class ProcessMonitor {
private:
    std::vector<ProcessInfo> processes;        // Liste des processus actifs
    std::map<int, double> prevCpuTimes;        // Temps CPU précédents pour le calcul
    long totalSystemTime;                      // Temps CPU total du système
    long prevTotalSystemTime;                  // Temps CPU total précédent

    // Méthodes privées
    void readProcessList();                    // Lit tous les processus du système
    double calculateCpuUsage(int pid, double prevTime); // Calcule l'utilisation CPU

public:
    ProcessMonitor();

    // Méthodes principales
    void update();                             // Met à jour les informations
    
    // Getters
    const std::vector<ProcessInfo>& getProcesses() const { return processes; }
    std::vector<ProcessInfo> getTopProcesses(int count = 5) const; // Retourne les processus les plus actifs
};

#endif // PROCESS_MONITOR_H