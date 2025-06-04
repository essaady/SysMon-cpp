#ifndef _CPUMONITOR_H
#define _CPUMONITOR_H
#include <string>

typedef struct cpu{
        float frequency;
        float frequencyMax;
        float usageCPU;
        float* usagePerCPU;
        short nbrCPU;
} cpu;

// Structure to hold CPU time values read from /proc/stat
typedef struct CpuTimes {
    int user = 0;
    int nice = 0;
    int system = 0;
    int idle = 0;
    int iowait = 0;
    int irq = 0;
    int softirq = 0;
    int steal = 0;
    int guest = 0;
    int guest_nice = 0;

    int totalIdleTime() const {
        return idle + iowait;
    }

    int totalTime() const {
        return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
    }
} CpuTimes;

class CpuMonitor {

    private:
        CpuTimes previousTimes;
        CpuTimes currentTimes;
        
    protected:
        // Vous pourrez ajouter des variables privées ici plus tard pour stocker l'état du CPU
        cpu CPU{};
        std::string rawCPU;
        CpuTimes readCpuTimes(); // read proc/stat
        void updateTimes();
        

    public:
        // Constructeur
        CpuMonitor();
        
        // Destructeur
        ~CpuMonitor();
        // DONE : Méthode pour récupérer le nombre des cœurs du CPU
        static short getCpuCoreNumber();
        // Méthode pour récupérer l'utilisation du CPU
        float getCpuUsage();
        // DONE: Méthode pour récupérer le fréquence du CPU
        float getCpuFreq();

        float getCpuMaxFreq();
        std::string getCpuInfo();

        bool update();
};

#endif 
