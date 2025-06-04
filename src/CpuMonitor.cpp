#ifndef CPUMONITOR_H
#define CPUMONITOR_H
#include "CpuMonitor.h"
#include <iostream>


class CpuMonitor {
private:
    struct CPU {
        short nbrCPU;
        float* usagePerCPU;
        float frequency;
        float frequencyMax;
    } cpu;

public:
    CpuMonitor();
    ~CpuMonitor();

    bool update();               
    float getCpuUsage();        
    float getCpuFreq();        
    float getCpuInfo();         
};

#endif



CpuMonitor::CpuMonitor() {
    cpu.nbrCPU = 4;  
    cpu.usagePerCPU = new float[cpu.nbrCPU]{20.5f, 35.2f, 50.1f, 10.0f};
    cpu.frequency = 2.4f;    
    cpu.frequencyMax = 3.5f;  
}

CpuMonitor::~CpuMonitor() {
    delete[] cpu.usagePerCPU;
}

bool CpuMonitor::update() {

    for (int i = 0; i < cpu.nbrCPU; ++i) {
        cpu.usagePerCPU[i] = 10.0f + i * 5.0f;
    }
    cpu.frequency = 2.5f; 
    return true;
}

float CpuMonitor::getCpuUsage() {
    float total = 0.0f;
    for (int i = 0; i < cpu.nbrCPU; ++i) {
        total += cpu.usagePerCPU[i];
    }
    return total / cpu.nbrCPU;
}

float CpuMonitor::getCpuFreq() {
    return cpu.frequency;
}

float CpuMonitor::getCpuInfo() {
    return cpu.frequencyMax;
}
