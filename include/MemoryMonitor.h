#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

class MemoryMonitor {
public:
    MemoryMonitor();

    void update();                  // Met à jour les valeurs mémoire
    double getUsedMemory() const;   // Retourne la mémoire utilisée (en Mo)
    double getTotalMemory() const;  // Retourne la mémoire totale (en Mo)

private:
    double usedMemory;
    double totalMemory;

    void parseProcMeminfo();
};

#endif // MEMORYMONITOR_H
