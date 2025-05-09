#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

CpuMonitor::CpuTimes CpuMonitor::readCpuTimes() {
    std::ifstream file("/proc/stat");
    std::string line;
    CpuTimes times;

    if (file.is_open() && std::getline(file, line)) {
        std::istringstream ss(line);
        std::string cpu;
        ss >> cpu; // skip 'cpu'

        ss >> times.user >> times.nice >> times.system >> times.idle
           >> times.iowait >> times.irq >> times.softirq;
    }

    return times;
}

double CpuMonitor::getUsage() {
    CpuTimes t1 = readCpuTimes();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CpuTimes t2 = readCpuTimes();

    unsigned long long totalDelta = t2.total() - t1.total();
    unsigned long long idleDelta = t2.idleTime() - t1.idleTime();

    if (totalDelta == 0) return 0.0;

    double cpuPercentage = 100.0 * (totalDelta - idleDelta) / totalDelta;
    return cpuPercentage;
}
