#include "CpuMonitor.h"
#include <windows.h>
#include <iostream>

CpuMonitor::CpuMonitor() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    cpu.nbrCPU = sysInfo.dwNumberOfProcessors;
    cpu.usagePerCPU = new float[cpu.nbrCPU];
    cpu.frequency = 2.5f;
    cpu.frequencyMax = 3.4f;
}

CpuMonitor::~CpuMonitor() {
    delete[] cpu.usagePerCPU;
}

bool CpuMonitor::update() {
    // Simuler la mise à jour pour l’instant
    for (int i = 0; i < cpu.nbrCPU; i++) {
        cpu.usagePerCPU[i] = 30.0f + (i * 5.0f); // valeur fictive
    }
    return true;
}

float CpuMonitor::getCpuUsage() {
    float total = 0;
    for (int i = 0; i < cpu.nbrCPU; i++) {
        total += cpu.usagePerCPU[i];
    }
    return total / cpu.nbrCPU;
}

float CpuMonitor::getCpuFreq() {
    return cpu.frequency;
}

float CpuMonitor::getCpuInfo() {
    return static_cast<float>(cpu.nbrCPU); // exemple temporaire
}
