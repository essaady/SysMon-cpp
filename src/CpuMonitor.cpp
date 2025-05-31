#include "CpuMonitor.h"
#include <fstream>
#include <sstream>

CpuMonitor::CpuMonitor() : lastIdleTime(0), lastTotalTime(0), usage(0.0) {
    update(); // initialise les valeurs au démarrage
}

void CpuMonitor::parseProcStat(unsigned long long& idleTime, unsigned long long& totalTime) {
    std::ifstream file("/proc/stat");
    std::string line;
    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream ss(line);
        std::string cpu;
        unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;

        ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        idleTime = idle + iowait;
        totalTime = user + nice + system + idle + iowait + irq + softirq + steal;
    }
}

void CpuMonitor::update() {
    unsigned long long idleTime, totalTime;
    parseProcStat(idleTime, totalTime);

    unsigned long long idleDelta = idleTime - lastIdleTime;
    unsigned long long totalDelta = totalTime - lastTotalTime;

    if (totalDelta != 0) {
        usage = (1.0 - (double)idleDelta / totalDelta) * 100.0;
    }

    lastIdleTime = idleTime;
    lastTotalTime = totalTime;
}

double CpuMonitor::getUsage() const {
    return usage;
}

