#include "ProcessMonitor.h"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

ProcessMonitor::ProcessMonitor() {
    totalMemoryMB = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGE_SIZE) / (1024 * 1024);
    clockTicks = sysconf(_SC_CLK_TCK);
}

bool ProcessMonitor::update() {
    processes.clear();

    DIR* dir = opendir("/proc");
    if (!dir) return false;

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type != DT_DIR) continue;

        std::string pidStr(entry->d_name);
        if (!std::all_of(pidStr.begin(), pidStr.end(), ::isdigit)) continue;

        int pid = std::stoi(pidStr);
        std::string statPath = "/proc/" + pidStr + "/stat";
        std::ifstream statFile(statPath);
        if (!statFile) continue;

        std::string comm;
        char state;
        long utime, stime, starttime;
        int ppid, uid;
        unsigned long vsize;
        long rss;

        std::string line;
        std::getline(statFile, line);
        std::istringstream iss(line);
        std::string dummy;
        iss >> dummy >> comm >> state;
        for (int i = 0; i < 10; ++i) iss >> dummy; // skip to utime
        iss >> utime >> stime;
        for (int i = 0; i < 7; ++i) iss >> dummy;
        iss >> starttime;

        // get UID from status
        std::ifstream statusFile("/proc/" + pidStr + "/status");
        uid = -1;
        if (statusFile) {
            while (std::getline(statusFile, line)) {
                if (line.find("Uid:") == 0) {
                    std::istringstream uiss(line);
                    std::string key;
                    uiss >> key >> uid;
                    break;
                }
            }
        }

        std::string user = (uid != -1) ? getUserFromUID(uid) : "unknown";
        std::string exePath = "/proc/" + pidStr + "/exe";
        char buf[512];
        ssize_t len = readlink(exePath.c_str(), buf, sizeof(buf)-1);
        std::string pathName = (len != -1) ? std::string(buf, len) : "unknown";

        AP ap;
        ap.user = user;
        ap.pid = pid;
        ap.cpu = getCpuUsage(pid);
        ap.memory = getMemoryUsage(pid);
        ap.time = getTimeFromTicks(utime + stime);
        ap.pathName = pathName;

        processes.push_back(ap);
    }

    closedir(dir);
    return true;
}

AP ProcessMonitor::getProcess(int index) {
    if (index >= 0 && index < processes.size())
        return processes[index];
    return {};
}

std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream oss;
    for (const auto& p : processes) {
        oss << "PID: " << p.pid
            << " | User: " << p.user
            << " | CPU: " << p.cpu << " %"
            << " | MEM: " << p.memory << " MB"
            << " | Time: " << p.time
            << " | Path: " << p.pathName
            << "\n";
    }
    return oss.str();
}

int ProcessMonitor::getProcessCount() {
    return processes.size();
}

std::string ProcessMonitor::getUserFromUID(int uid) {
    struct passwd* pw = getpwuid(uid);
    return pw ? pw->pw_name : "unknown";
}

float ProcessMonitor::getMemoryUsage(int pid) {
    std::ifstream status("/proc/" + std::to_string(pid) + "/status");
    std::string line;
    while (std::getline(status, line)) {
        if (line.find("VmRSS:") == 0) {
            std::istringstream iss(line);
            std::string key;
            int kb;
            iss >> key >> kb;
            return kb / 1024.0f;
        }
    }
    return 0.0f;
}

float ProcessMonitor::getCpuUsage(int pid) {
    // This is simplified; for detailed CPU usage you need deltas & total time
    return 0.0f; // Placeholder
}

std::string ProcessMonitor::getTimeFromTicks(long ticks) {
    int totalSec = ticks / clockTicks;
    int h = totalSec / 3600;
    int m = (totalSec % 3600) / 60;
    int s = totalSec % 60;
    char buf[16];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", h, m, s);
    return std::string(buf);
}
