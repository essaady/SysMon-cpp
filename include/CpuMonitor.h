#ifndef CPUMONITOR_H
#define CPUMONITOR_H

class CpuMonitor {
public:
    CpuMonitor();

    void update();             // Met à jour les valeurs CPU
    double getUsage() const;   // Retourne l'utilisation CPU en %

private:
    unsigned long long lastIdleTime;
    unsigned long long lastTotalTime;

    double usage;              // Utilisation CPU calculée
    void parseProcStat(unsigned long long& idleTime, unsigned long long& totalTime);
};

#endif // CPUMONITOR_H
