#include "..\include\CpuMonitor.h"

#ifdef _WIN32

#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

bool CpuMonitor::update() {

    static long counter = 0;
    CPU.usageCPU = 20.0f + (counter++ % 50);
    CPU.nbrCPU = GetSystemInfoWrapper();
    CPU.frequency = 2400.0f;   
    CPU.frequencyMax = 3200.0f; 
    rawCPU = "Simulated CPU data";
    return true;
}

int CpuMonitor::GetSystemInfoWrapper() {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return static_cast<int>(sysinfo.dwNumberOfProcessors);
}

float CpuMonitor::getCpuUsage() {
    return CPU.usageCPU;
}

float CpuMonitor::getCpuFreq() {
    return CPU.frequency;
}

std::string CpuMonitor::getCpuInfo() {
    std::ostringstream oss;
    oss << "CPU Usage: " << CPU.usageCPU << "%\n"
        << "Frequency: " << CPU.frequency << " MHz\n"
        << "Cores: " << CPU.nbrCPU;
    return oss.str();
}

#else  // LINUX

#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

bool CpuMonitor::update() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line); 

    std::istringstream iss(line);
    std::string cpuLabel;
    long user, nice, system, idle, iowait, irq, softirq, steal;
    iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    long idleTime = idle + iowait;
    long totalTime = user + nice + system + idleTime + irq + softirq + steal;

    static long prevIdle = 0, prevTotal = 0;
    long deltaIdle = idleTime - prevIdle;
    long deltaTotal = totalTime - prevTotal;

    if (deltaTotal == 0) return false;

    CPU.usageCPU = 100.0f * (1.0f - (float)deltaIdle / deltaTotal);
    prevIdle = idleTime;
    prevTotal = totalTime;

    CPU.nbrCPU = sysconf(_SC_NPROCESSORS_ONLN);
    CPU.frequency = getCurrentCpuFreq();
    CPU.frequencyMax = getMaxCpuFreq();

    rawCPU = line;
    return true;
}

float CpuMonitor::getCpuUsage() {
    return CPU.usageCPU;
}

float CpuMonitor::getCpuFreq() {
    return CPU.frequency;
}

std::string CpuMonitor::getCpuInfo() {
    std::ostringstream oss;
    oss << "CPU Usage: " << CPU.usageCPU << "%\n"
        << "Frequency: " << CPU.frequency << " MHz\n"
        << "Cores: " << CPU.nbrCPU;
    return oss.str();
}

float CpuMonitor::getCurrentCpuFreq() {
    std::ifstream file("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    float freq = 0.0f;
    if (file.is_open()) {
        file >> freq;
        freq /= 1000.0f;
    }
    return freq;
}

float CpuMonitor::getMaxCpuFreq() {
    std::ifstream file("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    float freq = 0.0f;
    if (file.is_open()) {
        file >> freq;
        freq /= 1000.0f;
    }
    return freq;
}

#endif
