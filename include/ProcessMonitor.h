#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <vector>
#include <string>

using namespace std;

// Structure pour contenir les informations sur un processus
struct ProcessInfo {
    int pid;                // Identifiant du processus
    string name;            // Nom du processus
    double cpuUsage;        // Utilisation du CPU en pourcentage
    double memoryUsageMB;   // Utilisation de la mémoire en Mo
};

// Classe ProcessMonitor pour surveiller les processus en cours
class ProcessMonitor {
public:
    // Constructeur de la classe
    ProcessMonitor();

    // Méthode pour mettre à jour les informations sur les processus
    void update();

    // Méthode pour obtenir la liste des processus
    const vector<ProcessInfo>& getProcesses() const;

private:
    // Méthode pour obtenir le temps total d'utilisation du CPU
    unsigned long long getTotalCpuTime() const;

    // Méthode pour lire les processus dans /proc
    void readProcesses();

    // Liste des processus surveillés
    vector<ProcessInfo> processes;

    // Dernier temps total du CPU pour calculer l'usage du CPU
    unsigned long long lastTotalCpuTime;
};

#endif // PROCESSMONITOR_H
