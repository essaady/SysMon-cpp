#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

// Constructeur
CpuMonitor::CpuMonitor() {
    // Initialisation des ressources si nécessaire
        CPU.frequencyMax = 0;

    // Compter le nombre de CPU 
    ifstream file("/proc/stat");
    string line;
    short count = 0;

    while (getline(file, line)) {
        if (line.find("cpu") == 0 && isdigit(line[3])) {
            count++;
        }
    }

    CPU.nbrCPU = count;
    CPU.usagePerCPU = new float[CPU.nbrCPU];
    memset(CPU.usagePerCPU, 0, CPU.nbrCPU * sizeof(float));
}

// Méthode pour récupérer l'utilisation du CPU (sera implémentée plus tard)
float CpuMonitor::getCpuUsage() {
    // Retourner une valeur factice pour l'instant (par exemple, 0.0)
    // Lire les temps 2 fois avec un délai
    previousTimes = readCpuTimes();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    currentTimes = readCpuTimes();

    long long prevIdle = previousTimes.totalIdleTime();
    long long prevTotal = previousTimes.totalTime();

    long long currIdle = currentTimes.totalIdleTime();
    long long currTotal = currentTimes.totalTime();

    long long deltaIdle = currIdle - prevIdle;
    long long deltaTotal = currTotal - prevTotal;

    float usage = 0.0;
    if (deltaTotal != 0) {
        usage = 100.0f * (deltaTotal - deltaIdle) / deltaTotal;
    }

    return usage;
    return 0.0;
}

// Destructeur
CpuMonitor::~CpuMonitor() {
    // Libération des ressources si nécessaire
    delete[] CPU.usagePerCPU;
}

bool CpuMonitor::update(){
    CPU.usageCPU  = getCpuUsage(); 
    CPU.frequency = getCpuFreq(); 
    rawCPU = getCpuInfo();
    return true;
}

float CpuMonitor::getCpuFreq(){
    ifstream file("/proc/cpuinfo");
    string line;

    while (getline(file, line)) {
        if (line.find("cpu MHz") != string::npos) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                float freq = stof(line.substr(pos + 1));
                return freq; // en MHz
            }
        }
    }

    return 0.0;
}

std::string CpuMonitor::getCpuInfo(){
    ifstream file("/proc/cpuinfo");
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

