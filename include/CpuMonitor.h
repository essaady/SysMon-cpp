#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <vector>
#include <string>

class CpuMonitor {
public:
    CpuMonitor();

    // Met à jour les statistiques CPU
    void update();

    // Renvoie l'utilisation du CPU en pourcentage (entre 0.0 et 100.0)
    double getCpuUsage() const;

private:
    // Structure pour stocker les temps CPU
    struct CpuTimes {
        unsigned long long user;
        unsigned long long nice;
        unsigned long long system;
        unsigned long long idle;
        unsigned long long iowait;
        unsigned long long irq;
        unsigned long long softirq;
        unsigned long long steal;

        unsigned long long total() const;
        unsigned long long idleAll() const;
    };

    // Statistiques précédentes et actuelles
    CpuTimes prevTimes;
    CpuTimes currentTimes;

    // Méthode pour lire /proc/stat
    CpuTimes readCpuTimes() const;
};

#endif // CPUMONITOR_H
