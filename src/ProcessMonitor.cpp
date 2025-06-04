#include "../include/ProcessMonitor.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <dirent.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <filesystem>
#include <algorithm>
#include <chrono>
#include <sstream>

ProcessMonitor::ProcessMonitor(int maxProcesses) 
    : maxProcessesToShow(maxProcesses), sortByCpu(true), sortByMemory(false) {
    update();
}

ProcessMonitor::~ProcessMonitor() {
}

bool ProcessMonitor::readProcessInfo(int pid, ProcessInfo& info) {
    info.pid = pid;
    info.timestamp = std::chrono::system_clock::now();
    
    // Read process status file to get UID and other details
    std::string statusPath = std::string(PROC_DIR) + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusPath);
    if (!statusFile.is_open()) {
        return false;
    }
    
    std::string line;
    int uid = -1;
    
    while (std::getline(statusFile, line)) {
        if (line.find("Uid:") == 0) {
            std::istringstream uidStream(line.substr(5));
            uidStream >> uid;
        } else if (line.find("Threads:") == 0) {
            std::istringstream threadsStream(line.substr(9));
            threadsStream >> info.threads;
        }
    }
    
    if (uid != -1) {
        info.user = getProcessUser(uid);
    } else {
        info.user = "unknown";
    }
    
    // Get command
    info.command = getProcessCommand(pid);
    
    // Get status
    info.status = getProcessStatus(pid);
    
    // Calculate CPU and memory usage
    info.cpu = calculateCpuUsage(pid);
    info.memory = calculateMemoryUsage(pid);
    
    return true;
}

std::string ProcessMonitor::getProcessUser(int uid) {
    struct passwd* pw = getpwuid(uid);
    if (pw) {
        return pw->pw_name;
    }
    return std::to_string(uid);
}

std::string ProcessMonitor::getProcessCommand(int pid) {
    std::string cmdlinePath = std::string(PROC_DIR) + std::to_string(pid) + "/cmdline";
    std::ifstream cmdlineFile(cmdlinePath);
    std::string cmdline;
    
    if (cmdlineFile.is_open()) {
        std::getline(cmdlineFile, cmdline);
        
        // Replace null characters with spaces
        for (char& c : cmdline) {
            if (c == '\0') c = ' ';
        }
        
        if (cmdline.empty()) {
            // Try to get process name from comm file
            std::string commPath = std::string(PROC_DIR) + std::to_string(pid) + "/comm";
            std::ifstream commFile(commPath);
            if (commFile.is_open()) {
                std::getline(commFile, cmdline);
                cmdline = "[" + cmdline + "]";
            } else {
                cmdline = "[unknown]";
            }
        }
    }
    
    // Limit command display length
    if (cmdline.length() > 50) {
        cmdline = cmdline.substr(0, 47) + "...";
    }
    
    return cmdline;
}

std::string ProcessMonitor::getProcessStatus(int pid) {
    std::string statusPath = std::string(PROC_DIR) + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusPath);
    if (statusFile.is_open()) {
        std::string line;
        while (std::getline(statusFile, line)) {
            if (line.find("State:") == 0) {
                return line.substr(7, 1);
            }
        }
    }
    return "?";
}

float ProcessMonitor::calculateCpuUsage(int pid) {
    // This is a simplified version. For accurate CPU usage, you'd need 
    // to calculate CPU time over a time interval
    std::string statPath = std::string(PROC_DIR) + std::to_string(pid) + "/stat";
    std::ifstream statFile(statPath);
    if (!statFile.is_open()) {
        return 0.0;
    }
    
    std::string content;
    std::getline(statFile, content);
    std::istringstream iss(content);
    
    // Skip the first 13 fields to get to utime
    std::string value;
    for (int i = 0; i < 13; i++) {
        iss >> value;
    }
    
    unsigned long long utime = 0, stime = 0;
    iss >> utime >> stime;
    
    // Calculate a pseudo CPU usage (in reality, a proper implementation would
    // track usage over time)
    return (utime + stime) / 100.0;
}

float ProcessMonitor::calculateMemoryUsage(int pid) {
    std::string statmPath = std::string(PROC_DIR) + std::to_string(pid) + "/statm";
    std::ifstream statmFile(statmPath);
    
    if (statmFile.is_open()) {
        unsigned long size = 0, resident = 0;
        statmFile >> size >> resident;
        
        // Convert pages to MB
        long pageSize = sysconf(_SC_PAGESIZE);
        float memoryUsageMB = (resident * pageSize) / (1024.0 * 1024.0);
        
        return memoryUsageMB;
    }
    
    return 0.0;
}

int ProcessMonitor::getProcessThreadCount(int pid) {
    std::string taskDir = std::string(PROC_DIR) + std::to_string(pid) + "/task";
    int count = 0;
    
    DIR* dir = opendir(taskDir.c_str());
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            if (entry->d_name[0] != '.') {
                count++;
            }
        }
        closedir(dir);
    }
    
    return count;
}

void ProcessMonitor::sortProcesses() {
    if (sortByCpu) {
        std::sort(processes.begin(), processes.end(), 
            [](const ProcessInfo& a, const ProcessInfo& b) { 
                return a.cpu > b.cpu; 
            });
    } else if (sortByMemory) {
        std::sort(processes.begin(), processes.end(), 
            [](const ProcessInfo& a, const ProcessInfo& b) { 
                return a.memory > b.memory; 
            });
    }
}

bool ProcessMonitor::update() {
    processes.clear();
    
    DIR* dir = opendir(PROC_DIR);
    if (!dir) {
        std::cerr << "Failed to open /proc directory" << std::endl;
        return false;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Check if the directory name is a number (PID)
        if (entry->d_type == DT_DIR) {
            char* endptr;
            long pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0') {  // Valid PID (numeric directory name)
                ProcessInfo info;
                if (readProcessInfo(pid, info)) {
                    processes.push_back(info);
                }
            }
        }
    }
    closedir(dir);
    
    // Sort processes
    sortProcesses();
    
    return true;
}

void ProcessMonitor::displayProcessInfo() {
    update();
    
    std::cout << "=== Process Information (Top " << std::min(maxProcessesToShow, (int)processes.size()) << ") ===\n";
    std::cout << std::left
              << std::setw(7) << "PID"
              << std::setw(10) << "USER"
              << std::setw(6) << "CPU%"
              << std::setw(10) << "MEM(MB)"
              << std::setw(5) << "THR"
              << std::setw(5) << "STAT"
              << "COMMAND\n";
              
    int count = 0;
    for (const auto& proc : processes) {
        if (count++ >= maxProcessesToShow) break;
        
        std::cout << std::left
                  << std::setw(7) << proc.pid
                  << std::setw(10) << (proc.user.length() > 9 ? proc.user.substr(0, 7) + ".." : proc.user)
                  << std::fixed << std::setprecision(1)
                  << std::setw(6) << proc.cpu
                  << std::setw(10) << proc.memory
                  << std::setw(5) << proc.threads
                  << std::setw(5) << proc.status
                  << proc.command << "\n";
    }
    std::cout << std::endl;
}

const std::vector<ProcessInfo>& ProcessMonitor::getProcesses() const {
    return processes;
}

void ProcessMonitor::setSortByCpu(bool sort) {
    sortByCpu = sort;
    sortByMemory = !sort;
}

void ProcessMonitor::setSortByMemory(bool sort) {
    sortByMemory = sort;
    sortByCpu = !sort;
}

void ProcessMonitor::setMaxProcessesToShow(int count) {
    maxProcessesToShow = count;
}

float ProcessMonitor::getTotalCpuUsage() const {
    float total = 0.0f;
    for (const auto& proc : processes) {
        total += proc.cpu;
    }
    return total;
}

float ProcessMonitor::getTotalMemoryUsage() const {
    float total = 0.0f;
    for (const auto& proc : processes) {
        total += proc.memory;
    }
    return total;
}

std::string ProcessMonitor::exportProcessInfoAsText() const {
    std::stringstream ss;
    
    ss << "PROCESS INFORMATION\n";
    ss << "-------------------\n";
    ss << "Total processes: " << processes.size() << "\n\n";
    
    ss << std::left
       << std::setw(7) << "PID"
       << std::setw(15) << "USER"
       << std::setw(10) << "CPU%"
       << std::setw(10) << "MEM(MB)"
       << std::setw(10) << "THREADS"
       << std::setw(10) << "STATUS"
       << "COMMAND\n";
       
    for (int i = 0; i < std::min(maxProcessesToShow, (int)processes.size()); i++) {
        const auto& proc = processes[i];
        ss << std::left
           << std::setw(7) << proc.pid
           << std::setw(15) << proc.user
           << std::fixed << std::setprecision(1)
           << std::setw(10) << proc.cpu
           << std::setw(10) << proc.memory
           << std::setw(10) << proc.threads
           << std::setw(10) << proc.status
           << proc.command << "\n";
    }
    
    return ss.str();
}

std::string ProcessMonitor::exportProcessInfoAsCSV() const {
    std::stringstream ss;
    
    // CSV header
    ss << "PID,USER,CPU%,MEMORY_MB,THREADS,STATUS,COMMAND\n";
    
    for (const auto& proc : processes) {
        ss << proc.pid << ","
           << "\"" << proc.user << "\","
           << std::fixed << std::setprecision(1) << proc.cpu << ","
           << proc.memory << ","
           << proc.threads << ","
           << "\"" << proc.status << "\","
           << "\"" << proc.command << "\"\n";
    }
    
    return ss.str();
}