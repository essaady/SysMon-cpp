#include "../include/ProcessMonitor.h"
#include "../include/SysMon.h"
#include <dirent.h>
#include <pwd.h>
#include <iomanip>

ProcessMonitor::ProcessMonitor() {
    update();
}

ProcessMonitor::~ProcessMonitor() {
}

bool ProcessMonitor::parseProcessInfo(int pid, activeProcesses& process) {
    process.pid = pid;
    
    std::string statusPath = PROC_PATH + std::to_string(pid) + "/status";
    std::string cmdLinePath = PROC_PATH + std::to_string(pid) + "/cmdline";
    
    std::ifstream statusFile(statusPath);
    if (!statusFile.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(statusFile, line)) {
        if (line.substr(0, 4) == "Uid:") {
            std::istringstream iss(line);
            std::string key;
            int uid;
            iss >> key >> uid;
            process.user = getProcessUser(uid);
        }
    }
    
    std::ifstream cmdlineFile(cmdLinePath);
    if (cmdlineFile.is_open()) {
        std::string cmdLine;
        std::getline(cmdlineFile, cmdLine);
        
        if (cmdLine.empty()) {
            std::string commPath = PROC_PATH + std::to_string(pid) + "/comm";
            std::ifstream commFile(commPath);
            if (commFile.is_open()) {
                std::getline(commFile, process.command);
            } else {
                process.command = "[unknown]";
            }
        } else {
            process.command = cmdLine;
            size_t nullPos = process.command.find('\0');
            if (nullPos != std::string::npos) {
                process.command = process.command.substr(0, nullPos);
            }
        }
    }
    
    process.cpu = getProcessCpuUsage(pid);
    process.memory = getProcessMemoryUsage(pid);
    
    return true;
}

std::string ProcessMonitor::getProcessUser(int uid) {
    struct passwd* pw = getpwuid(uid);
    if (pw) {
        return pw->pw_name;
    }
    return std::to_string(uid);
}

float ProcessMonitor::getProcessCpuUsage(int pid) {
    return 0.0; // Placeholder - implement actual CPU calculation
}

float ProcessMonitor::getProcessMemoryUsage(int pid) {
    std::string statmPath = PROC_PATH + std::to_string(pid) + "/statm";
    std::ifstream statmFile(statmPath);
    
    if (statmFile.is_open()) {
        long rss = 0;
        statmFile >> rss; // pages
        statmFile >> rss; // resident set size
        
        long pageSize = sysconf(_SC_PAGESIZE);
        float memoryUsageMB = (rss * pageSize) / (1024.0 * 1024.0);
        
        return memoryUsageMB;
    }
    
    return 0.0;
}

bool ProcessMonitor::update() {
    processes.clear();
    
    DIR* dir = opendir(PROC_PATH);
    if (!dir) {
        return false;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Check if the directory name is a number (i.e., a PID)
        if (entry->d_type == DT_DIR) {
            char* endptr;
            int pid = strtol(entry->d_name, &endptr, 10);
            
            if (*endptr == '\0') { // Valid PID
                activeProcesses process;
                if (parseProcessInfo(pid, process)) {
                    processes.push_back(process);
                }
            }
        }
    }
    
    closedir(dir);
    
    return true;
}

const std::vector<activeProcesses>& ProcessMonitor::getProcesses() const {
    return processes;
}

void ProcessMonitor::displayProcessInfo(int limit) {
    update();
    
    std::cout << "=== Process Information (Top " << limit << ") ===\n";
    std::cout << std::left << std::setw(7) << "PID" 
              << std::setw(15) << "USER" 
              << std::setw(10) << "CPU%" 
              << std::setw(10) << "MEM(MB)" 
              << "COMMAND" << std::endl;
    
    int count = 0;
    for (const auto& proc : processes) {
        if (count++ >= limit) break;
        
        std::cout << std::left << std::setw(7) << proc.pid
                  << std::setw(15) << proc.user
                  << std::setw(10) << std::fixed << std::setprecision(1) << proc.cpu
                  << std::setw(10) << std::fixed << std::setprecision(1) << proc.memory
                  << proc.command << std::endl;
    }
    
    std::cout << std::endl;
}