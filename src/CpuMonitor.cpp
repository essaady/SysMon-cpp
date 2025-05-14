#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
using namespace std;


CpuMonitor::CpuMonitor() {
    prevTimes = readCpuTimes();
}

// Lit les temps CPU depuis /proc/stat
CpuMonitor::CpuTimes CpuMonitor::readCpuTimes() const {
    ifstream file("/proc/stat");
    string line;
    CpuTimes times = {};

    if (file.is_open()) {
        getline(file, line);
        istringstream iss(line);

        string cpuLabel;
        iss >> cpuLabel; // "cpu"

        iss >> times.user >> times.nice >> times.system >> times.idle
            >> times.iowait >> times.irq >> times.softirq >> times.steal;
    }

    return times;
}

void CpuMonitor::update() {
    prevTimes = currentTimes;
    currentTimes = readCpuTimes();
}

double CpuMonitor::getCpuUsage() const {
    unsigned long long prevIdle = prevTimes.idleAll();
    unsigned long long currIdle = currentTimes.idleAll();

    unsigned long long prevTotal = prevTimes.total();
    unsigned long long currTotal = currentTimes.total();

    unsigned long long totalDelta = currTotal - prevTotal;
    unsigned long long idleDelta = currIdle - prevIdle;

    if (totalDelta == 0) return 0.0;

    return 100.0 * (1.0 - static_cast<double>(idleDelta) / totalDelta);
}

// Calcule le temps total (user + nice + system + idle + ...)
unsigned long long CpuMonitor::CpuTimes::total() const {
    return user + nice + system + idle + iowait + irq + softirq + steal;
}

unsigned long long CpuMonitor::CpuTimes::idleAll() const {
    return idle + iowait;
}
