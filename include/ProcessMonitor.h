#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <algorithm>
#include <iomanip>

struct ProcessInfo {
    int pid;
    std::string command;
    double cpuUsage;
};

long getUptime() {
    std::ifstream file("/proc/uptime");
    double uptime = 0.0;
    if (file.is_open()) {
        file >> uptime;
        file.close();
    }
    return static_cast<long>(uptime);
}

std::vector<ProcessInfo> getProcessesCPUUsage() {
    std::vector<ProcessInfo> processes;
    DIR* dir = opendir("/proc");
    struct dirent* entry;
    long uptime = getUptime();
    long clk_tck = sysconf(_SC_CLK_TCK);

    while ((entry = readdir(dir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {
            int pid = atoi(entry->d_name);
            std::string statPath = "/proc/" + std::string(entry->d_name) + "/stat";
            std::ifstream statFile(statPath);
            if (statFile.is_open()) {
                std::string line;
                std::getline(statFile, line);
                statFile.close();

                std::istringstream iss(line);
                std::string comm;
                char c;
                std::string dummy;
                long utime, stime, cutime, cstime, starttime;

                iss >> pid >> comm;
                // Supprimer les parenthèses autour de la commande
                if (comm.front() == '(' && comm.back() == ')')
                    comm = comm.substr(1, comm.size() - 2);

                // Avancer jusqu'au champ 14
                for (int i = 3; i <= 13; ++i) iss >> dummy;
                iss >> utime >> stime >> cutime >> cstime;

                for (int i = 18; i < 22; ++i) iss >> dummy;
                iss >> starttime;

                long totalTime = utime + stime + cutime + cstime;
                double seconds = uptime - (starttime / (double)clk_tck);
                double cpuUsage = 0.0;
                if (seconds > 0)
                    cpuUsage = 100.0 * ((totalTime / (double)clk_tck) / seconds);

                processes.push_back({ pid, comm, cpuUsage });
            }
        }
    }
    closedir(dir);
    return processes;
}

void displayTopCPUProcesses(int topN = 5) {
    auto processes = getProcessesCPUUsage();

    std::sort(processes.begin(), processes.end(),
              [](const ProcessInfo& a, const ProcessInfo& b) {
                  return a.cpuUsage > b.cpuUsage;
              });

    std::cout << std::left << std::setw(8) << "PID"
              << std::setw(25) << "Command"
              << std::setw(10) << "CPU (%)" << std::endl;
    std::cout << std::string(45, '-') << std::endl;

    for (int i = 0; i < std::min(topN, (int)processes.size()); ++i) {
        std::cout << std::left << std::setw(8) << processes[i].pid
                  << std::setw(25) << processes[i].command
                  << std::fixed << std::setprecision(2)
                  << processes[i].cpuUsage << std::endl;
    }
}
