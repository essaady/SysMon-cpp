#include "ProcessMonitor.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <dirent.h>
#include <pwd.h>
#include <unistd.h>
#include <cstring>

ProcessMonitor::ProcessMonitor() 
    : systemUptime(0), prevSystemUptime(0), hertz(sysconf(_SC_CLK_TCK)) 
{
    prevSystemUptime = readSystemUptime();
    update();
}

ProcessMonitor::~ProcessMonitor() {
    // Cleanup if needed
}

bool ProcessMonitor::update() {
    systemUptime = readSystemUptime();
    unsigned long long elapsedTime = systemUptime - prevSystemUptime;

    processes.clear();

    DIR* dir = opendir("/proc");
    if (!dir) {
        std::cerr << "Failed to open /proc directory" << std::endl;
        return false;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            char* endptr;
            int pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0') {
                ProcessInfo info = readProcessInfo(pid);
                info.cpuUsage = calculateProcessCpuUsage(pid, info.startTime, elapsedTime);
                processes.push_back(info);
            }
        }
    }

    closedir(dir);

    std::sort(processes.begin(), processes.end(),
              [](const ProcessInfo& a, const ProcessInfo& b) {
                  return a.cpuUsage > b.cpuUsage;
              });

    prevSystemUptime = systemUptime;

    return true;
}

std::vector<ProcessInfo> ProcessMonitor::getProcesses() const {
    return processes;
}

std::vector<ProcessInfo> ProcessMonitor::getTopCPUProcesses(int n) const {
    if (n <= 0 || n >= static_cast<int>(processes.size())) {
        return processes;
    }
    return std::vector<ProcessInfo>(processes.begin(), processes.begin() + n);
}

std::vector<ProcessInfo> ProcessMonitor::getTopMemoryProcesses(int n) const {
    if (n <= 0 || n >= static_cast<int>(processes.size())) {
        return processes;
    }

    std::vector<ProcessInfo> memProcesses = processes;
    std::sort(memProcesses.begin(), memProcesses.end(),
              [](const ProcessInfo& a, const ProcessInfo& b) {
                  return a.memoryUsage > b.memoryUsage;
              });
    return std::vector<ProcessInfo>(memProcesses.begin(), memProcesses.begin() + n);
}

ProcessInfo ProcessMonitor::getProcessInfo(int pid) const {
    for (const auto& process : processes) {
        if (process.pid == pid) {
            return process;
        }
    }
    return ProcessInfo{};
}

std::string ProcessMonitor::exportAsText(int limit) const {
    std::ostringstream oss;
    oss << "Processes: " << processes.size() << " total\n";
    oss << std::setw(7) << "PID" << " | "
        << std::setw(15) << "USER" << " | "
        << std::setw(8) << "CPU%" << " | "
        << std::setw(8) << "MEM%" << " | "
        << std::setw(8) << "RSS(MB)" << " | "
        << std::setw(8) << "VSZ(MB)" << " | "
        << std::setw(5) << "PRI" << " | "
        << std::setw(5) << "NI" << " | "
        << std::setw(5) << "THR" << " | "
        << std::setw(5) << "S" << " | "
        << "COMMAND\n";

    int count = 0;
    for (const auto& process : processes) {
        if (limit > 0 && count >= limit) break;

        double rssMB = process.rss / 1024.0;
        double vszMB = process.vsz / 1024.0;

        oss << std::setw(7) << process.pid << " | "
            << std::setw(15) << process.user << " | "
            << std::setw(7) << std::fixed << std::setprecision(1) << process.cpuUsage << "% | "
            << std::setw(7) << std::fixed << std::setprecision(1) << process.memoryUsage << "% | "
            << std::setw(8) << std::fixed << std::setprecision(1) << rssMB << " | "
            << std::setw(8) << std::fixed << std::setprecision(1) << vszMB << " | "
            << std::setw(5) << process.priority << " | "
            << std::setw(5) << process.nice << " | "
            << std::setw(5) << process.threads << " | "
            << std::setw(5) << process.state << " | "
            << process.name << "\n";

        ++count;
    }
    return oss.str();
}

std::string ProcessMonitor::exportAsCSV(int limit) const {
    std::ostringstream oss;
    oss << "PID,USER,CPU%,MEM%,RSS(MB),VSZ(MB),PRI,NI,THR,STATE,COMMAND\n";

    int count = 0;
    for (const auto& process : processes) {
        if (limit > 0 && count >= limit) break;

        double rssMB = process.rss / 1024.0;
        double vszMB = process.vsz / 1024.0;

        oss << process.pid << ","
            << process.user << ","
            << std::fixed << std::setprecision(1) << process.cpuUsage << ","
            << std::fixed << std::setprecision(1) << process.memoryUsage << ","
            << std::fixed << std::setprecision(1) << rssMB << ","
            << std::fixed << std::setprecision(1) << vszMB << ","
            << process.priority << ","
            << process.nice << ","
            << process.threads << ","
            << process.state << ","
            << process.name << "\n";

        ++count;
    }
    return oss.str();
}

ProcessInfo ProcessMonitor::readProcessInfo(int pid) {
    ProcessInfo info;
    info.pid = pid;

    std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream statFile(statPath);
    unsigned long long totalTime = 0;
    if (statFile.is_open()) {
        std::string line;
        if (std::getline(statFile, line)) {
            int dummy;
            char state;
            unsigned long utime, stime;
            unsigned long long starttime;
            unsigned long long vsize;
            long rss;
            int priority, nice, threads;

            size_t start = line.find('(');
            size_t end = line.rfind(')');
            if (start != std::string::npos && end != std::string::npos) {
                info.name = line.substr(start + 1, end - start - 1);

                std::string restOfLine = line.substr(end + 2);
                std::istringstream restIss(restOfLine);

                restIss >> state >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy
                        >> dummy >> dummy >> dummy >> dummy >> utime >> stime >> dummy
                        >> dummy >> priority >> nice >> threads >> dummy >> starttime
                        >> vsize >> rss;

                totalTime = utime + stime
