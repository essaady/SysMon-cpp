
#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

cpu CPU; // variable globale représentant le CPU

string rawCPU; // contenu brut de /proc/stat

bool updateCPU() {
    // Lire /proc/stat deux fois pour calculer le pourcentage
    unsigned long idle1, total1;
    readCPUStats(idle1, total1);
    this_thread::sleep_for(chrono::milliseconds(100));
    unsigned long idle2, total2;
    readCPUStats(idle2, total2);

    unsigned long deltaIdle = idle2 - idle1;
    unsigned long deltaTotal = total2 - total1;

    if (deltaTotal == 0) return false;

    CPU.usageCPU = 100.0f * (1.0f - (float)deltaIdle / deltaTotal);
    return true;
}

void readCPUStats(unsigned long &idleTime, unsigned long &totalTime) {
    ifstream file("/proc/stat");
    string line;
    getline(file, line); // Lire ligne "cpu ..."

    rawCPU = line;

    istringstream iss(line);
    string cpuLabel;
    unsigned long user, nice, system, idle, iowait, irq, softirq, steal;

    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    idleTime = idle + iowait;
    unsigned long nonIdle = user + nice + system + irq + softirq + steal;
    totalTime = idleTime + nonIdle;
}

float getCpuUsage() {
    return CPU.usageCPU;
}

float getCpuFreq() {
    ifstream file("/proc/cpuinfo");
    string line;
    while (getline(file, line)) {
        if (line.find("cpu MHz") != string::npos) {
            size_t pos = line.find(":");
            CPU.frequency = stof(line.substr(pos + 1));
            return CPU.frequency;
        }
    }
    return 0.0f;
}

string getCpuInfo() {
    ifstream file("/proc/cpuinfo");
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}