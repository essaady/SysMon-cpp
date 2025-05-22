#ifndef CPUMONITOR_H
#define CPUMONITOR_H

class CpuMonitor {
public:
    // Constructeur
    CpuMonitor();
    
    // Méthode pour récupérer l'utilisation du CPU
    double getCpuUsage();
    
    // Destructeur
    ~CpuMonitor();

private:
    // Vous pourrez ajouter des variables privées ici plus tard pour stocker l'état du CPU
};

#endif // CPUMONITOR_H

