#include "Process.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <pwd.h>
#include <dirent.h>
#include <sys/stat.h>

Process::Process() : nbrProcess(0) {}

Process::~Process() {}

bool Process::update() {
    processes.clear();
    nbrProcess = 0;

    DIR* proc = opendir("/proc");
    if (!proc) return false;

    struct dirent* entry;
    while ((entry = readdir(proc)) != nullptr) {
        if (!isdigit(entry->d_name[0])) continue;

        int pid = std::stoi(entry->d_name);
        std::string stat_path = "/proc/" + std::to_string(pid) + "/stat";
        std::ifstream stat_file(stat_path);
        if (!stat_file.is_open()) continue;

        ActiveProcess ap;
        ap.pid = pid;
        ap.cpu = getCPUUsage(pid);
        ap.memory = getMemoryUsage(pid);
        ap.time = getProcessTime(pid);
        ap.pathName = getPathName(pid);

        std::string status_path = "/proc/" + std::to_string(pid) + "/status";
        std::ifstream status_file(status_path);
        std::string line;
        while (std::getline(status_file, line)) {
            if (line.find("Uid:") == 0) {
                std::istringstream iss(line);
                std::string label;
                uid_t uid;
                iss >> label >> uid;
                ap.user = getUserFromUID(uid);
                break;
            }
        }

        processes.push_back(ap);
        nbrProcess++;
    }

    closedir(proc);
    return true;
}

Process::ActiveProcess Process::getProcess(int index) const {
    if (index < 0 || index >= nbrProcess) return ActiveProcess{};
    return processes[index];
}

std::string Process::getProcessInfo() const {
    std::ostringstream oss;
    for (const auto& p : processes) {
        oss << "PID: " << p.pid << ", CPU: " << p.cpu << "%, RAM: " << p.memory
            << "MB, User: " << p.user << ", Time: " << p.time
            << ", Path: " << p.pathName << "\n";
    }
    return oss.str();
}

std::string Process::getProcessRaw() const {
    std::ostringstream oss;
    for (const auto& p : processes) {
        oss << p.pid << "," << p.cpu << "," << p.memory << ","
            << p.user << "," << p.time << "," << p.pathName << "\n";
    }
    return oss.str();
}

std::string Process::getUserFromUID(uid_t uid) const {
    struct passwd* pw = getpwuid(uid);
    return pw ? pw->pw_name : "unknown";
}

float Process::getCPUUsage(int pid) const {
    return 0.0f;
}

float Process::getMemoryUsage(int pid) const {
    std::ifstream status("/proc/" + std::to_string(pid) + "/status");
    std::string line;
    while (std::getline(status, line)) {
        if (line.find("VmRSS:") == 0) {
            std::istringstream iss(line);
            std::string label;
            float memKB;
            iss >> label >> memKB;
            return memKB / 1024.0f;
        }
    }
    return 0.0f;
}

std::string Process::getProcessTime(int pid) const {
    std::ifstream stat("/proc/" + std::to_string(pid) + "/stat");
    std::string token;
    for (int i = 0; i < 22; ++i) stat >> token;
    return token;
}

std::string Process::getPathName(int pid) const {
    char path[1024];
    std::string exe_link = "/proc/" + std::to_string(pid) + "/exe";
    ssize_t len = readlink(exe_link.c_str(), path, sizeof(path) - 1);
    if (len != -1) {
        path[len] = '\0';
        return std::string(path);
    }
    return "unknown";
}
