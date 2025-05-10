#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

unsigned long long CpuMonitor::CpuSnapshot::total() const {
    return user + nice + system + idle + iowait + irq + softirq;
}

unsigned long long CpuMonitor::CpuSnapshot::idleTime() const {
    return idle + iowait;
}

CpuMonitor::CpuSnapshot CpuMonitor::takeSnapshot() const {
    std::ifstream statFile("/proc/stat");
    std::string line;
    CpuSnapshot snapshot{};

    if (statFile.is_open() && std::getline(statFile, line)) {
        std::istringstream iss(line);
        std::string cpu;
        iss >> cpu;
        iss >> snapshot.user >> snapshot.nice >> snapshot.system >> snapshot.idle
            >> snapshot.iowait >> snapshot.irq >> snapshot.softirq;
    }

    return snapshot;
}

double CpuMonitor::getUsage() {
    CpuSnapshot first = takeSnapshot();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CpuSnapshot second = takeSnapshot();

    unsigned long long totalDiff = second.total() - first.total();
    unsigned long long idleDiff = second.idleTime() - first.idleTime();

    if (totalDiff == 0) return 0.0;

    return 100.0 * (totalDiff - idleDiff) / totalDiff;
}
