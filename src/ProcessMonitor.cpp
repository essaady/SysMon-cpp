#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>

struct ProcessInfo {
    int pid;
    std::string command;
    double cpuPercent;
};

// Helper to get system uptime (in seconds)
double getSystemUptime() {
    std::ifstream file("/proc/uptime");
    double uptime = 0.0;
    if (file.is_open()) {
        file >> uptime;
        file.close();
    }
    return uptime;
}

// Read and calculate CPU usage from /proc/[pid]/stat
std::vector<ProcessInfo> getTopCPUProcesses(int topN = 5) {
    std::vector<ProcessInfo> processes;
    DIR* procDir = opendir("/proc");
    if (!procDir) return processes;

    double uptime = getSystemUptime();
    long ticksPerSecond = sysconf(_SC_CLK_TCK);

    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        if (!isdigit(entry->d_name[0])) continue;

        int pid = atoi(entry->d_name);
        std::string statPath = "/proc/" + std::string(entry->d_name) + "/stat";
        std::ifstream statFile(statPath);
        if (!statFile.is_open()) continue;

        std::string line;
        std::getline(statFile, line);
        statFile.close();

        std::istringstream iss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (iss >> token)
            tokens.push_back(token);

        if (tokens.size() < 22) continue;

        std::string command = tokens[1];
        long utime = std::stol(tokens[13]);
        long stime = std::stol(tokens[14]);
        long starttime = std::stol(tokens[21]);

        long totalTime = utime + stime;
        double seconds = uptime - (starttime / (double)ticksPerSecond);
        if (seconds <= 0) continue;

        double cpuUsage = 100.0 * ((totalTime / (double)ticksPerSecond) / seconds);

        processes.push_back({pid, command, cpuUsage});
    }

    closedir(procDir);

    std::sort(processes.begin(), processes.end(), [](const ProcessInfo& a, const ProcessInfo& b) {
        return a.cpuPercent > b.cpuPercent;
    });

    if (processes.size() > topN)
        processes.resize(topN);

    return processes;
}

// For testing
void displayTopProcesses() {
    std::vector<ProcessInfo> top = getTopCPUProcesses();
    std::cout << "PID\tCOMMAND\t\tCPU (%)\n";
    std::cout << "---------------------------------\n";
    for (const auto& proc : top) {
        std::cout << proc.pid << "\t" << proc.command << "\t\t" << proc.cpuPercent << "\n";
    }
}
