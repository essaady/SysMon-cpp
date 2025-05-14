#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

class MemoryMonitor {
public:
    MemoryMonitor();

    // Met à jour les informations mémoire
    void update();

    // Renvoie la mémoire totale en Mo
    double getTotalMemoryMB() const;

    // Renvoie la mémoire utilisée en Mo
    double getUsedMemoryMB() const;

    // Renvoie le pourcentage d'utilisation de la mémoire
    double getMemoryUsagePercent() const;

private:
    unsigned long totalMemoryKB;
    unsigned long freeMemoryKB;
    unsigned long availableMemoryKB;
    unsigned long buffersKB;
    unsigned long cachedKB;

    // Lit les données depuis /proc/meminfo
    void readMemInfo();
};

#endif // MEMORYMONITOR_H
