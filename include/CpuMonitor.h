#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <string>
#include <fstream>
#include <array>

/**
 * @brief Classe pour surveiller l'utilisation du CPU sur les systèmes Linux
 * 
 * Cette classe permet de lire et calculer l'utilisation du CPU en temps réel
 * en utilisant les statistiques du fichier /proc/stat.
 */
class CpuMonitor {
public:
    CpuMonitor();
    ~CpuMonitor() = default;

    // Obtenir l'utilisation actuelle du CPU en pourcentage
    double getCpuUsage();
    
    // Obtenir une chaîne formatée de l'utilisation du CPU
    std::string getFormattedUsage();

private:
    // Structure pour stocker les statistiques CPU
    struct CpuStats {
        unsigned long user;      // Temps passé en mode utilisateur
        unsigned long nice;      // Temps passé en mode utilisateur avec priorité nice
        unsigned long system;    // Temps passé en mode système
        unsigned long idle;      // Temps passé en mode inactif
        unsigned long iowait;    // Temps passé en attente d'E/S
        unsigned long irq;       // Temps passé à gérer les interruptions
        unsigned long softirq;   // Temps passé à gérer les softirqs
        unsigned long steal;     // Temps volé par d'autres systèmes virtuels
        unsigned long guest;     // Temps passé à exécuter des systèmes virtuels
        unsigned long guest_nice;// Temps passé à exécuter des systèmes virtuels avec priorité nice
    };

    // Statistiques CPU précédentes pour le calcul des deltas
    CpuStats prevStats;
    bool firstRun;  // Indique si c'est la première exécution

    // Lire les statistiques CPU actuelles depuis /proc/stat
    CpuStats readCpuStats();
    
    // Calculer le pourcentage d'utilisation du CPU à partir des statistiques
    double calculateCpuUsage(const CpuStats& current, const CpuStats& previous);
};

#endif // CPU_MONITOR_H
