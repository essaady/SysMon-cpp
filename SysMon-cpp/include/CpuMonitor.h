#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <string>

struct cpu {
    float frequency = 0;
    float frequencyMax = 0;
    float usageCPU = 0;
    float* usagePerCPU = nullptr; // Peut être implémenté plus tard
    short nbrCPU = 0;
};

extern struct cpu CPU;
extern std::string rawCPU;

bool updateCPU();
float getCpuUsage();
float getCpuFreq();
std::string getCpuInfo();

#endif