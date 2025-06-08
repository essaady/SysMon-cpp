#pragma once
#ifndef _CPUMONITOR_H
#define _CPUMONITOR_H
#include <string>
#include <cstdint>

typedef struct cpu{
        float frequency;
        float frequencyMax;
        float usageCPU;
        float* usagePerCPU;
        short nbrCPU;
    } cpu;

// Structure to hold CPU time values read from /proc/stat
typedef struct CpuTimes {
    long long user = 0;
    long long nice = 0;
    long long system = 0;
    long long idle = 0;
    long long iowait = 0;
    long long irq = 0;
    long long softirq = 0;
    long long steal = 0;
    long long guest = 0;
    long long guest_nice = 0;

    long long totalIdleTime() const {
        return idle + iowait;
    }

    long long totalTime() const {
        return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
    }
} CpuTimes;

class CpuMonitor {

    private:
        CpuTimes previousTimes;
        CpuTimes currentTimes;
        
    protected:
        // Vous pourrez ajouter des variables privées ici plus tard pour stocker l'état du CPU
        cpu CPU;

        std::string rawCPU;
        CpuTimes readCpuTimes(); // read proc/stat
        void updateTimes(); 
        uint64_t getSnap(std::string calc);
        

    public:
        // Constructeur
        CpuMonitor();
        
        // Destructeur
        ~CpuMonitor();

        // Méthode pour récupérer l'utilisation du CPU
        float getCpuUsage();
        
        float getCpuFreq();

        std::string getCpuInfo();

        bool update();

        float calcCpuUsage(int log, int updateInterval=5e5);
};

#endif 
