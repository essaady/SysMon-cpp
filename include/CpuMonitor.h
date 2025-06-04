#ifndef CPUMONITOR_H //Si ce fichier n’a pas encore été inclus dans le programme, 
// alors inclue-le maintenant. Sinon, ignore-le.
#define CPUMONITOR_H

#include <vector>
#include <string>

struct CPU {
    float usage;         // Pourcentage d'utilisation du cœur
    float frequency;     // Fréquence actuelle
    float frequencyMax;  // Fréquence maximale
};

class CpuMonitor {
private:
    short nbrCPU;                  // Nombre de cœurs
    std::vector<CPU> cpus;         // Liste des cœurs

public:
 // Constructeur
    CpuMonitor();                 
// Mettre à jour les infos du CPU
    bool update();
    // Utilisation moyenne              
    float getCpuUsage();         
    float getCpuFreq();           // Fréquence moyenne
    std::string getCpuInfo();     // Infos générales

   int getNbrCPU() const;
    std::vector<CPU> getCpus() const;
};

#endif
