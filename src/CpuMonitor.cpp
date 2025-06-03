#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

CpuMonitor::CpuMonitor() {}

CpuMonitor::CpuTimes CpuMonitor::readCpuTimes() {
    std::ifstream file("/proc/stat");
    std::string line;
    CpuTimes times{};

    if (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string cpu;
        iss >> cpu >> times.user >> times.nice >> times.system >> times.idle
            >> times.iowait >> times.irq >> times.softirq >> times.steal;
    }

    return times;
}

double CpuMonitor::getCpuUsage() {
    CpuTimes t1 = readCpuTimes();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CpuTimes t2 = readCpuTimes();

    long long total1 = t1.total();
    long long total2 = t2.total();
    long long idle1 = t1.idleTime();
    long long idle2 = t2.idleTime();

    long long deltaTotal = total2 - total1;
    long long deltaIdle = idle2 - idle1;

    if (deltaTotal == 0) return 0.0;
    return 100.0 * (deltaTotal - deltaIdle) / deltaTotal;
}
