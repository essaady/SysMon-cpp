#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>
using namespace std;

class MemoryMonitor {
private:
    unsigned long total_memory; //la memoire totale en KB
    unsigned long free_memory; //la memoire libre en KB
    bool readMemoryInfo(); //lire les donnees de memoire depuis /proc/meminfo

public:
    //constructeur par defaut 
    MemoryMonitor();

    //mettre a jour les donnees de memoire
    bool update();

    //methodes pour obtenir les informations de memoire (getters)
    unsigned long getTotalMemory() const;
    unsigned long getFreeMemory() const;
    unsigned long getUsedMemory() const;

    //affiche les informations de memoire
    void display() const;

    //formater les valeurs de memoire en GB
    string formatMemory(unsigned long memory_kb) const;
};

#endif
