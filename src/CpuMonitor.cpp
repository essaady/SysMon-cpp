#include "../include/CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <thread>

CpuMonitor::CpuMonitor() {
    update();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    update();
}

CpuMonitor::~CpuMonitor() {
    // Rien à faire ici
}

bool CpuMonitor::update() {
    prevCpuData = currentCpuData;

    if (!readCpuData()) {
        return false;
    }

    for (const auto& pair : currentCpuData) {
        const std::string& cpuName = pair.first;
        const CPUData& current = pair.second;

        if (prevCpuData.find(cpuName) != prevCpuData.end()) {
            const CPUData& prev = prevCpuData[cpuName];
            cpuUsage[cpuName] = calculateUsage(prev, current);
        } else {
            cpuUsage[cpuName] = 0.0;
        }
    }

    return true;
}

double CpuMonitor::getTotalUsage() const {
    auto it = cpuUsage.find("cpu");
    if (it != cpuUsage.end()) {
        return it->second;
    }
    return 0.0;
}

std::map<std::string, double> CpuMonitor::getCoreUsage() const {
    std::map<std::string, double> coreUsage;
    for (const auto& pair : cpuUsage) {
        if (pair.first != "cpu") {
            coreUsage[pair.first] = pair.second;
        }
    }
    return coreUsage;
}

std::string CpuMonitor::exportAsText() const {
    std::ostringstream oss;
    oss << "CPU Usage:\n";
    oss << "  Total: " << std::fixed << std::setprecision(2) << getTotalUsage() << "%\n";
    oss << "  Cores:\n";
    for (const auto& pair : getCoreUsage()) {
        oss << "    " << pair.first << ": " << std::fixed << std::setprecision(2) << pair.second << "%\n";
    }
    return oss.str();
}

std::string CpuMonitor::exportAsCSV() const {
    std::ostringstream oss;
    oss << "CPU,Usage(%)\n";
    oss << "Total," << std::fixed << std::setprecision(2) << getTotalUsage() << "\n";
    for (const auto& pair : getCoreUsage()) {
        oss << pair.first << "," << std::fixed << std::setprecision(2) << pair.second << "\n";
    }
    return oss.str();
}

bool CpuMonitor::readCpuData() {
    std::ifstream statFile("/proc/stat");
    if (!statFile.is_open()) {
        std::cerr << "Failed to open /proc/stat" << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(statFile, line)) {
        if (line.compare(0, 3, "cpu") == 0) {
            std::istringstream iss(line);
            std::string cpuName;
            CPUData data = {}; // initialize to zero

            iss >> cpuName
                >> data.user >> data.nice >> data.system >> data.idle
                >> data.iowait >> data.irq >> data.softirq >> data.steal
                >> data.guest >> data.guest_nice;

            if (iss.fail()) {
                std::cerr << "Error parsing /proc/stat line: " << line << std::endl;
                continue;
            }

            currentCpuData[cpuName] = data;
        }
    }

    statFile.close();
    return true;
}

double CpuMonitor::calculateUsage(const CPUData& prev, const CPUData& current) const {
    unsigned long long prevIdle = prev.idle + prev.iowait;
    unsigned long long currIdle = current.idle + current.iowait;

    unsigned long long prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
    unsigned long long currNonIdle = current.user + current.nice + current.system + current.irq + current.softirq + current.steal;

    unsigned long long prevTotal = prevIdle + prevNonIdle;
    unsigned long long currTotal = currIdle + currNonIdle;

    if (currTotal < prevTotal) { // protection contre overflow
        return 0.0;
    }

    unsigned long long totalDiff = currTotal - prevTotal;
    unsigned long long idleDiff = currIdle - prevIdle;

    if (totalDiff == 0) {
        return 0.0;
    }

    double usage = 100.0 * (totalDiff - idleDiff) / totalDiff;
    return usage;
}
