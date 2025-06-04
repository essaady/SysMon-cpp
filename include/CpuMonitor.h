#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <cmath>

using namespace std;

class CpuMonitor {
private:
    struct CPU {
        short nbrCPU;
        float usagePerCPU;
        float frequency;
        float frequencyMax;
    } cpu;

    // Pour calculer usage
    unsigned long long lastTotal = 0;
    unsigned long long lastIdle = 0;

public:
    CpuMonitor() {
        cpu.nbrCPU = sysconf(_SC_NPROCESSORS_ONLN);
        cpu.frequencyMax = getMaxFrequency();
    }

    // ------------ 🔄 update() ------------
    bool update() {
        ifstream file("/proc/st
