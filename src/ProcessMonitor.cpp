#include "../include/ProcessMonitor.h"
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>

ProcessMonitor::ProcessMonitor() {
    PID = nullptr;
    nbrProcess = 0;
    update(); // Initial update
}

ProcessMonitor::~ProcessMonitor() {
    if (PID) {
        delete[] PID;
    }
}

bool ProcessMonitor::update() {
    AP.clear();
    
    DIR* procDir = opendir("/proc");
    if (!procDir) {
        return false;
    }
    
    struct dirent* entry;
    std::vector<int> pids;
    
    // Get all process IDs
    while ((entry = readdir(procDir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            std::string dirname = entry->d_name;
            if (std::all_of(dirname.begin(), dirname.end(), ::isdigit)) {
                pids.push_back(std::stoi(dirname));
            }
        }
    }
    closedir(procDir);
    
    nbrProcess = pids.size();
    
    // Update PID array
    if (PID) {
        delete[] PID;
    }
    PID = new int[nbrProcess];
    std::copy(pids.begin(), pids.end(), PID);
    
    // Get detailed information for each process
    for (int pid : pids) {
        activeProcesses proc;
        std::string procPath = "/proc/" + std::to_string(pid);
        
        // Read /proc/[pid]/stat for CPU and memory info
        std::ifstream statFile(procPath + "/stat");
        if (statFile.is_open()) {
            std::string line;
            std::getline(statFile, line);
            std::istringstream iss(line);
            
            std::vector<std::string> fields;
            std::string field;
            while (iss >> field) {
                fields.push_back(field);
            }
            
            if (fields.size() >= 24) {
                // CPU time calculation (simplified)
                long utime = std::stol(fields[13]);
                long stime = std::stol(fields[14]);
                long totalTime = utime + stime;
                
                // Memory usage (RSS in pages * page size)
                long rss = std::stol(fields[23]);
                long pageSize = sysconf(_SC_PAGESIZE);
                proc.memory = (float)(rss * pageSize) / (1024 * 1024); // MB
                
                // CPU percentage (simplified calculation)
                proc.cpu = (float)totalTime / 100.0f; // This is a simplified version
                
                // Process time
                long clockTicks = sysconf(_SC_CLK_TCK);
                long totalTimeSec = totalTime / clockTicks;
                proc.time.hours = totalTimeSec / 3600;
                proc.time.minutes = (totalTimeSec % 3600) / 60;
                proc.time.seconds = totalTimeSec % 60;
            }
            statFile.close();
        }
        
        // Read /proc/[pid]/status for owner info
        std::ifstream statusFile(procPath + "/status");
        if (statusFile.is_open()) {
            std::string line;
            while (std::getline(statusFile, line)) {
                if (line.substr(0, 4) == "Uid:") {
                    std::istringstream iss(line);
                    std::string label;
                    uid_t uid;
                    if (iss >> label >> uid) {
                        struct passwd* pw = getpwuid(uid);
                        if (pw) {
                            proc.user = pw->pw_name;
                        } else {
                            proc.user = std::to_string(uid);
                        }
                    }
                    break;
                }
            }
            statusFile.close();
        }
        
        // Read /proc/[pid]/cmdline for command line
        std::ifstream cmdlineFile(procPath + "/cmdline");
        if (cmdlineFile.is_open()) {
            std::string cmdline;
            std::getline(cmdlineFile, cmdline);
            
            // Replace null characters with spaces
            for (char& c : cmdline) {
                if (c == '\0') c = ' ';
            }
            
            if (cmdline.empty()) {
                // Try to get from /proc/[pid]/comm
                std::ifstream commFile(procPath + "/comm");
                if (commFile.is_open()) {
                    std::getline(commFile, cmdline);
                    cmdline = "[" + cmdline + "]";
                    commFile.close();
                }
            }
            
            proc.pathName = cmdline.empty() ? "unknown" : cmdline;
            cmdlineFile.close();
        }
        
        AP[pid] = proc;
    }
    
    return true;
}

std::map<int, activeProcesses> ProcessMonitor::getProcess(int limit) {
    auto topProcs = getTopProcessesByCpu(limit);
    std::map<int, activeProcesses> result;
    
    for (const auto& proc : topProcs) {
        result[proc.first] = proc.second;
    }
    
    return result;
}

std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream oss;
    oss << "Total Processes: " << nbrProcess << "\n\n";
    oss << std::left << std::setw(8) << "PID" 
        << std::setw(10) << "USER" 
        << std::setw(8) << "CPU%" 
        << std::setw(10) << "MEM(MB)" 
        << std::setw(10) << "TIME" 
        << "COMMAND\n";
    oss << std::string(70, '-') << "\n";
    
    auto topProcs = getTopProcessesByCpu(20);
    for (const auto& proc : topProcs) {
        oss << std::left << std::setw(8) << proc.first
            << std::setw(10) << proc.second.user.substr(0, 9)
            << std::setw(8) << std::fixed << std::setprecision(1) << proc.second.cpu
            << std::setw(10) << std::fixed << std::setprecision(1) << proc.second.memory
            << std::setw(10) << std::setfill('0') << proc.second.time.hours << ":"
            << std::setw(2) << proc.second.time.minutes << ":"
            << std::setw(2) << proc.second.time.seconds << std::setfill(' ')
            << " " << proc.second.pathName.substr(0, 30) << "\n";
    }
    
    return oss.str();
}

std::string ProcessMonitor::getProcessRaw() {
    std::ostringstream oss;
    for (const auto& proc : AP) {
        oss << proc.first << "," << proc.second.user << "," 
            << proc.second.cpu << "," << proc.second.memory << ","
            << proc.second.time.hours << ":" << proc.second.time.minutes 
            << ":" << proc.second.time.seconds << ","
            << proc.second.pathName << "\n";
    }
    return oss.str();
}

std::vector<std::pair<int, activeProcesses>> ProcessMonitor::getTopProcessesByCpu(int limit) {
    std::vector<std::pair<int, activeProcesses>> procs(AP.begin(), AP.end());
    
    std::sort(procs.begin(), procs.end(), 
              [](const auto& a, const auto& b) {
                  return a.second.cpu > b.second.cpu;
              });
    
    if (limit > 0 && procs.size() > limit) {
        procs.resize(limit);
    }
    
    return procs;
}

std::vector<std::pair<int, activeProcesses>> ProcessMonitor::getTopProcessesByMemory(int limit) {
    std::vector<std::pair<int, activeProcesses>> procs(AP.begin(), AP.end());
    
    std::sort(procs.begin(), procs.end(), 
              [](const auto& a, const auto& b) {
                  return a.second.memory > b.second.memory;
              });
    
    if (limit > 0 && procs.size() > limit) {
        procs.resize(limit);
    }
    
    return procs;
}