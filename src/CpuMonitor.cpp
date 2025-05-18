#include "../include/CpuMonitor.h"
#include <sstream>
#include <iomanip>
#include <iostream>

// Constructeur : initialise les statistiques précédentes
CpuMonitor::CpuMonitor() : firstRun(true) {
    // Initialiser les statistiques précédentes
    prevStats = readCpuStats();
}

// Lecture des statistiques CPU depuis /proc/stat
CpuMonitor::CpuStats CpuMonitor::readCpuStats() {
    CpuStats stats = {};
    std::ifstream statFile("/proc/stat");
    std::string line;
    
    if (std::getline(statFile, line)) {
        std::istringstream iss(line);
        std::string cpu;
        iss >> cpu; // Ignorer le label "cpu"
        
        // Lire toutes les valeurs de statistiques
        iss >> stats.user >> stats.nice >> stats.system >> stats.idle
            >> stats.iowait >> stats.irq >> stats.softirq >> stats.steal
            >> stats.guest >> stats.guest_nice;
    }
    
    return stats;
}

// Calcul de l'utilisation CPU en pourcentage
double CpuMonitor::calculateCpuUsage(const CpuStats& current, const CpuStats& previous) {
    // Calculer le temps total passé
    unsigned long prevTotal = previous.user + previous.nice + previous.system + 
                            previous.idle + previous.iowait + previous.irq + 
                            previous.softirq + previous.steal;
    
    unsigned long currentTotal = current.user + current.nice + current.system + 
                               current.idle + current.iowait + current.irq + 
                               current.softirq + current.steal;
    
    // Calculer le temps d'inactivité
    unsigned long prevIdle = previous.idle + previous.iowait;
    unsigned long currentIdle = current.idle + current.iowait;
    
    // Calculer les différences (deltas)
    unsigned long totalDelta = currentTotal - prevTotal;
    unsigned long idleDelta = currentIdle - prevIdle;
    
    // Éviter la division par zéro
    if (totalDelta == 0) return 0.0;
    
    // Calculer le pourcentage d'utilisation CPU
    // Formule : (1 - temps_inactif / temps_total) * 100
    return 100.0 * (1.0 - static_cast<double>(idleDelta) / totalDelta);
}

// Obtenir l'utilisation CPU actuelle
double CpuMonitor::getCpuUsage() {
    CpuStats currentStats = readCpuStats();
    
    // Pour la première exécution, initialiser les stats précédentes
    if (firstRun) {
        prevStats = currentStats;
        firstRun = false;
        return 0.0;
    }
    
    // Calculer l'utilisation et mettre à jour les stats précédentes
    double usage = calculateCpuUsage(currentStats, prevStats);
    prevStats = currentStats;
    
    return usage;
}

// Obtenir une chaîne formatée de l'utilisation CPU
std::string CpuMonitor::getFormattedUsage() {
    double usage = getCpuUsage();
    std::ostringstream oss;
    // Formater avec 2 décimales
    oss << "CPU Usage: " << std::fixed << std::setprecision(2) << usage << "%";
    return oss.str();
}
