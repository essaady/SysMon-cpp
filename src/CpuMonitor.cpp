#include "../include/CpuMonitor.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <bits/ostream.tcc>
using namespace std;

// Constructeur
CpuMonitor::CpuMonitor() {
    // Initialisation des ressources si nécessaire
    CPU.frequencyMax = 0; //To-Do 
    CPU.nbrCPU = getCpuCoreNumber();  // Pour initialiser le nombre de cœurs du CPU
}

// Méthode pour récupérer le nombre de cœurs du CPU
short CpuMonitor::getCpuCoreNumber() {
    // Pour informer : Cette méthode de récupération des données (utilisée dans cette fonction), vise à obtenir la sortie d'une command shell.
    FILE *pipe = popen("grep -c '^processor' /proc/cpuinfo", "r");
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        pclose(pipe);
    }
    return std::stoi(buffer);
}

CpuTimes readCpuTimes() {
    int user = 0, nice = 0, system = 0, idle = 0, iowait = 0, irq = 0, softirq = 0, steal = 0, guest = 0, guest_nice = 0;
    char buffer[128];
    CpuTimes times;
    for (int i = 2; i <= 11; i++) {
        FILE* pipe = popen(("head -n 1 /proc/stat | awk '{print $" + std::to_string(i) + "}'").c_str(), "r");
        if (pipe) {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                switch (i) {
                    case 1:
                        times.user = std::stoi(buffer);
                        break;
                    case 2:
                        times.nice = std::stoi(buffer);
                        break;
                    case 3:
                        times.system = std::stoi(buffer);
                        break;
                    case 4:
                        times.idle = std::stoi(buffer);
                        break;
                    case 5:
                        times.iowait = std::stoi(buffer);
                        break;
                    case 6:
                        times.irq = std::stoi(buffer);
                        break;
                    case 7:
                        times.softirq = std::stoi(buffer);
                        break;
                    case 8:
                        times.steal = std::stoi(buffer);
                        break;
                    case 9:
                        times.guest = std::stoi(buffer);
                        break;
                    case 10:
                        times.guest_nice = std::stoi(buffer);
                        break;
                    default: break;
                }
                i++;
            }
        }
        pclose(pipe);
    }
    return times;
}

void CpuMonitor::updateTimes() {
    previousTimes = currentTimes;
    currentTimes = readCpuTimes();
}

// Méthode pour récupérer l'utilisation du CPU (sera implémentée plus tard)
float CpuMonitor::getCpuUsage() {

    // Calculate total and idle times
    long start_total = previousTimes.totalTime();
    long end_total = currentTimes.totalTime();
    long start_idle = previousTimes.totalIdleTime();
    long end_idle = currentTimes.totalIdleTime();

    // Calculate CPU usage percentage
    long total_diff = end_total - start_total;
    long idle_diff = end_idle - start_idle;

    if (total_diff == 0) {
        return 0.0; // Avoid division by zero
    }

    return ((total_diff - idle_diff) / static_cast<double>(total_diff)) * 100.0;
}

// Destructeur
CpuMonitor::~CpuMonitor() {
    // Libération des ressources si nécessaire
    exit(0);
}

bool CpuMonitor::update(){
    CPU.usageCPU  = getCpuUsage();
    CPU.frequency = getCpuFreq();
    CPU.frequencyMax = getCpuMaxFreq();
    rawCPU = getCpuInfo();
    return true;
}

// Méthodes pour récupérer la fréquence et la fréquence maximale du CPU
float CpuMonitor::getCpuMaxFreq() {
    std::string cmd = "bash -c 'head --silent -n 1 /sys/devices/system/cpu/cpu{0.." + std::to_string(CPU.nbrCPU - 1) + "}/cpufreq/cpuinfo_max_freq'";
    FILE *pipe = popen(cmd.c_str(), "r");
    float sumCpuMaxFreq = 0;
    char buffer[128];
    if (!pipe) return sumCpuMaxFreq;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        sumCpuMaxFreq += std::stof(buffer);
    }
    pclose(pipe);
    return (sumCpuMaxFreq/CPU.nbrCPU)/1e6;
}

float CpuMonitor::getCpuFreq() {
    FILE *pipe = popen("grep 'cpu MHz' /proc/cpuinfo | awk {'print $4'}", "r");
    float sumCpuFreq = 0;
    char buffer[128];
    if (!pipe) return sumCpuFreq;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        sumCpuFreq += std::stof(buffer);
    }
    pclose(pipe);
    return (sumCpuFreq/CPU.nbrCPU)/1e6;
}

// DONE: Récupérer Les informations du CPU
std::string CpuMonitor::getCpuInfo() {
    return "";
}

int main() {

    return 0;
}