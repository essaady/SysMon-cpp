#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <cstring>

CpuMonitor::CpuMonitor(){
    cpu.nbrCPU = std::thread::hardware_concurrency();
    cpu.usagePerCPU = new float[cpu.nbrCPU];
    std::memset(cpu.usagePerCPU, 0, sizeof(float) * cpu.nbrCPU);
    cpu.frequency = 0;
    cpu.frequencyMax = 0;
} 

CpuMonitor::~CpuMonitor() {
    delete[] cpu.usagePerCPU;
}

bool CpuMonitor::update() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    int index = 0;
static unsigned long long previousIdle[64] = {0};
static unsigned long long previousTotal[64] = {0};


while (std::getline(file, line)) {
        if (line.rfind("cpu", 0) == 0 && line[3] != ' ') {
            std::istringstream ss(line);
            std::string label;
            unsigned long long user, nice, system, idle;

            ss >> label >> user >> nice >> system >> idle;

            unsigned long long total = user + nice + system + idle;
            unsigned long long totalDiff = total - previousTotal[index];
            unsigned long long idleDiff = idle - previousIdle[index];

            previousTotal[index] = total;
            previousIdle[index] = idle;
            if (totalDiff != 0) {
                cpu.usagePerCPU[index] = 100.0 * (totalDiff - idleDiff) / totalDiff;
            } else {
                cpu.usagePerCPU[index] = 0;
            }

            index++;
            if (index >= cpu.nbrCPU) break;
        }
    }
    file.close();

     std::ifstream freqFile("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    if (freqFile.is_open()) {
        float freq;
        freqFile >> freq;
        cpu.frequency = freq / 1000.0f; // Convert from kHz to MHz
        freqFile.close();
    }
    std::ifstream maxFreqFile("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (maxFreqFile.is_open()) {
        float freqMax;
        maxFreqFile >> freqMax;
        cpu.frequencyMax = freqMax / 1000.0f;
        maxFreqFile.close();
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

std::string CpuMonitor::getCpuInfo() {
    std::ostringstream output;
    output << "Nombre de coeurs : " << cpu.nbrCPU << "\n";
    output << "Frequence actuelle : " << cpu.frequency << " MHz\n";
    output << "Frequence maximale : " << cpu.frequencyMax << " MHz\n";

    for (int i = 0; i < cpu.nbrCPU; i++) {
        output << "Core " << i << " : " << cpu.usagePerCPU[i] << "%\n";
    }

    return output.str();
}
