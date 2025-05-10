#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

struct CpuTimes {
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;

    unsigned long long total() const {
        return user + nice + system + idle + iowait + irq + softirq + steal;
    }

    unsigned long long idleTime() const {
        return idle + iowait;
    }
};

CpuTimes readCpuTimes() {
    std::ifstream file("/proc/stat");
    std::string line;
    CpuTimes times = {};

    if (file.is_open()) {
        std::getline(file, line);
        std::istringstream iss(line);
        std::string cpu;
        iss >> cpu >> times.user >> times.nice >> times.system >> times.idle
            >> times.iowait >> times.irq >> times.softirq >> times.steal;
    }
    return times;
}

float CpuMonitor::getCpuUsage(int delayMs) {
    CpuTimes t1 = readCpuTimes();
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
    CpuTimes t2 = readCpuTimes();

    unsigned long long totalDiff = t2.total() - t1.total();
    unsigned long long idleDiff = t2.idleTime() - t1.idleTime();

    if (totalDiff == 0) return 0.0f;
    return 100.0f * (1.0f - (float)idleDiff / totalDiff);
}
