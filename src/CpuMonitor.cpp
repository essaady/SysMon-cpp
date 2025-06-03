#include <iostream>
#include <vector>

class CpuMonitor {
private:
    struct CPU {
        short nbrCPU;
        float* usagePerCPU;
        float frequency;
        float frequencyMax;
    } cpu;

public:
    CpuMonitor() {
        cpu.nbrCPU = 4;  // Simulation avec 4 cœurs
        cpu.usagePerCPU = new float[cpu.nbrCPU]{20.5f, 35.2f, 50.1f, 10.0f};
        cpu.frequency = 2.4f;     
        cpu.frequencyMax = 3.5f;   
    }

    ~CpuMonitor() {
        delete[] cpu.usagePerCPU;
    }

    bool update() {
        // Simuler une mise à jour des données CPU
        for (int i = 0; i < cpu.nbrCPU; ++i) {
            cpu.usagePerCPU[i] = 10.0f + i * 5.0f;  // Valeurs fictives
        }
        cpu.frequency = 2.5f;  // Simuler une fréquence actuelle
        return true;
    }

    float getCpuUsage() {
        float total = 0.0f;
        for (int i = 0; i < cpu.nbrCPU; ++i) {
            total += cpu.usagePerCPU[i];
        }
        return total / cpu.nbrCPU;  /
    }

    float getCpuFreq() {
        return cpu.frequency;
    }

    float getCpuInfo() {
        return cpu.frequencyMax;
    }
};
