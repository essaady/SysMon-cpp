#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
//Hamza Tahiri
CpuMonitor::CpuMonitor() {
    readCPUData();
    prevData = currentData;
}

bool CpuMonitor::update() {
    prevData = currentData;
    return readCPUData();
}

bool CpuMonitor::readCPUData() {
    std::ifstream file("/proc/stat");
    if (!file.is_open()) return false;

    std::string line;
    currentData.clear();

    while (std::getline(file, line)) {
        if (line.substr(0, 3) != "cpu") break;

        std::istringstream iss(line);
        std::string cpu;
        CpuData data;
        iss >> cpu >> data.user >> data.nice >> data.system >> data.idle
            >> data.iowait >> data.irq >> data.softirq;

        currentData[cpu] = data;
    }

    for (const auto& [cpu, curr] : currentData) {
        const auto& prev = prevData[cpu];
        usage[cpu] = calculateUsage(prev, curr);
    }

    return true;
}

double CpuMonitor::calculateUsage(const CpuData& prev, const CpuData& curr) const {
    unsigned long long prevIdle = prev.idle + prev.iowait;
    unsigned long long currIdle = curr.idle + curr.iowait;

    unsigned long long prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq;
    unsigned long long currNonIdle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq;

    unsigned long long prevTotal = prevIdle + prevNonIdle;
    unsigned long long currTotal = currIdle + currNonIdle;

    double totald = currTotal - prevTotal;
    double idled = currIdle - prevIdle;

    if (totald == 0) return 0.0;
    return (totald - idled) / totald * 100.0;
}

double CpuMonitor::totalUsage() const {
    if (usage.find("cpu") != usage.end())
        return usage.at("cpu");
    return 0.0;
}

std::map<std::string, double> CpuMonitor::coreUsage() const {
    return usage;
}
