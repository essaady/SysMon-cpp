#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <iostream>

using namespace std;

// ------------------ Constructeur ------------------
CpuMonitor::CpuMonitor() {
    CPU.nbrCPU = sysconf(_SC_NPROCESSORS_ONLN);
    CPU.usagePerCPU = new float[CPU.nbrCPU];
    CPU.frequencyMax = getCpuFreq();  // approximation si cpuinfo_max_freq absent
    CPU.frequency = CPU.frequencyMax;
    CPU.usageCPU = 0.0;
    updateTimes(); // initialise les temps
}

// ------------------ Destructeur ------------------
CpuMonitor::~CpuMonitor() {
    delete[] CPU.usagePerCPU;
}

// ------------------ Lecture des temps CPU ------------------
CpuTimes CpuMonitor::readCpuTimes() {
    ifstream file("/proc/stat");
    string line;
    CpuTimes times;

    if (file.is_open()) {
        getline(file, line);
        istringstream ss(line);
        string cpuLabel;
        ss >> cpuLabel;
        ss >> times.user >> times.nice >> times.system >> times.idle
           >> times.iowait >> times.irq >> times.softirq
           >> times.steal >> times.guest >> times.guest_nice;
    }

    return times;
}

// ------------------ Mise à jour des temps ------------------
void CpuMonitor::updateTimes() {
    previousTimes = currentTimes;
    currentTimes = readCpuTimes();
}

// ------------------ Calcul de l'utilisation CPU ------------------
float CpuMonitor::getCpuUsage() {
    updateTimes();
    long long prevIdle = previousTimes.totalIdleTime();
    long long idle = currentTimes.totalIdleTime();

    long long prevTotal = previousTimes.totalTime();
    long long total = currentTimes.totalTime();

    long long deltaTotal = total - prevTotal;
    long long deltaIdle = idle - prevIdle;

    if (deltaTotal == 0) return 0.0;

    return 100.0f * (1.0f - ((float)deltaIdle / deltaTotal));
}

// ------------------ Récupération de la fréquence CPU ------------------
float CpuMonitor::getCpuFreq() {
    ifstream file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    float freq = 0.0;

    if (file.is_open()) {
        file >> freq;
        file.close();
        freq /= 1000.0f; // Hz → MHz
    } else {
        // fallback : lire depuis /proc/cpuinfo
        file.open("/proc/cpuinfo");
        string line;
        while (getline(file, line)) {
            if (line.find("cpu MHz") != string::npos) {
                size_t pos = line.find(":");
                if (pos != string::npos) {
                    freq = stof(line.substr(pos + 1));
                    break;
                }
            }
        }
        file.close();
    }

    return freq;
}

// ------------------ Récupération de la description CPU brute ------------------
string CpuMonitor::getCpuInfo() {
    ifstream file("/proc/cpuinfo");
    string line, result;

    while (getline(file, line)) {
        result += line + "\n";
    }

    file.close();
    return result;
}

// ------------------ Mise à jour globale ------------------
bool CpuMonitor::update() {
    CPU.usageCPU = getCpuUsage();
    CPU.frequency = getCpuFreq();
    rawCPU = getCpuInfo();

    // Affichage temporaire dans le terminal
    std::cout << "CPU Usage      : " << CPU.usageCPU << " %" << std::endl;
    std::cout << "CPU Frequency  : " << CPU.frequency << " MHz" << std::endl;
    std::cout << "CPU Max Freq   : " << CPU.frequencyMax << " MHz" << std::endl;
    std::cout << "Number of CPUs : " << CPU.nbrCPU << std::endl;

    return true;
}

