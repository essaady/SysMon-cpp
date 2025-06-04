#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>
using namespace std;

class CpuMonitor {
private:
    struct cpu {
        float frequency;
        float frequencyMax;
        float usageCPU;
        float* usagePerCPU;
        short nbrCPU;
    } CPU;

    string rawCPU;

public:
    bool update();
    float getCpuUsage() const;
    float getCpuFreq() const;
    string getCpuInfo() const;
};

//le diagramme UML était respecté d'une manière stricte 

#endif 