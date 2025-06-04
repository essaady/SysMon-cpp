#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>
#include <atomic>

using namespace std;

class CpuMonitor {
private:
    // Attributs principaux (encapsulation)
    double currentUsage;
    double previousIdle;
    double previousTotal;
    string cpuInfo;
    bool isMonitoring;
    int updateInterval;

    vector<double> usageHistory;        // Historique des usages
    thread monitoringThread;            // Thread de surveillance

    // Méthodes internes privées
    vector<long> readCpuStats();                       // Lire les stats CPU (/proc/stat)
    double calculateUsage(const vector<long>& stats);  // Calculer % usage
    void parseCpuInfo();                               // Lire /proc/cpuinfo
    void monitoringLoop();                             // Boucle de mise à jour auto

public:
    // Constructeurs et destructeur
    CpuMonitor();                  // Constructeur par défaut
    CpuMonitor(int interval);     // Constructeur avec intervalle
    ~CpuMonitor();                // Destructeur

    // Méthodes principales
    void startMonitoring();       // Lancer le monitoring
    void stopMonitoring();        // Arrêter le monitoring
    void updateCpuUsage();        // Mettre à jour manuellement
    void displayCpuInfo() const;  // Afficher les infos CPU

    // Getters
    double getCurrentUsage() const;
    string getCpuInfo() const;
    bool getMonitoringStatus() const;
    int getUpdateInterval() const;

    // Setters
    void setUpdateInterval(int interval);

    // Utilitaires
    void exportToFile(const string& filename) const;  // Exporter vers fichier
    void printUsageHistory() const;                   // Afficher historique
    string formatUsageString() const;                 // Barre visuelle

    // Méthode statique
    static string getSystemInfo();                    // Infos système
};

#endif // CPUMONITOR_H
