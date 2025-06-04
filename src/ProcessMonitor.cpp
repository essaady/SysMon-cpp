#include "ProcessMonitor.h"
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <cctype>
#include <unistd.h>
#include <pwd.h>
#include <algorithm>
#include <ctime>
#include <iomanip>

ProcessMonitor::ProcessMonitor() {
    PID = 0;
    nbrProcess = 0;
}

bool ProcessMonitor::isNumeric(const std::string& str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

time_t ProcessMonitor::getBootTime() {
    ifstream stat_file("/proc/stat");
    string line;
    while (getline(stat_file, line)) {
        if (line.find("btime ") == 0) {
            return stoll(line.substr(6));
        }
    }
    return time(nullptr);
}

string ProcessMonitor::getUsername(int uid) {
    struct passwd* pw = getpwuid(uid);
    if (pw) {
        return string(pw->pw_name);
    }
    return to_string(uid);
}

bool ProcessMonitor::update() {
    ap.clear();
    vector<int> pids;
    vector<AP> allProcesses;
    
    // get PIDs
        for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
            if (entry.is_directory()) {
                string dirname = entry.path().filename().string();
                if (isNumeric(dirname)) { 
                    pids.push_back(stoi(dirname)); 
                }
            }
        }
    
    time_t boot_time = getBootTime();
    long clock_ticks = sysconf(_SC_CLK_TCK);

    // collect data for each PID
    for (int pid : pids) {
        AP process;
        string proc_path = "/proc/" + to_string(pid) + "/";

        process.cpu = 0.0;
        process.memory = 0.0;
        process.pathName = "Unknown";
        process.user = "Unknown";
        process.time.startTime = "Unknown";
        process.time.duration = "Unknown";

        // get process path 
        try {
            process.pathName = filesystem::read_symlink(proc_path + "exe").string();
        } catch (...) {
            ifstream cmdline_file(proc_path + "cmdline");
            if (cmdline_file.is_open()) {
                getline(cmdline_file, process.pathName);
                for (char& c : process.pathName) {
                    if (c == '\0') c = ' ';
                }
                if (process.pathName.empty()) {
                    process.pathName = "[kernel thread]";
                }
            }
        }

        ifstream status_file(proc_path + "status");
        if (status_file.is_open()) {
            string line;
            while (getline(status_file, line)) {
                if (line.find("VmRSS:") == 0) {
                    stringstream ss(line.substr(line.find(":") + 1));
                    float kb;
                    if (ss >> kb) { 
                        process.memory = kb / 1024.0; // from KB to MB
                    }
                }
                if (line.find("Uid:") == 0) {
                    stringstream ss(line.substr(line.find(":") + 1));
                    int uid;
                    if (ss >> uid) {
                        process.user = getUsername(uid);
                    }
                }
            }
        }

        ifstream stat_file(proc_path + "stat");
        if (stat_file.is_open()) {
            string stat_content;
            getline(stat_file, stat_content);
            vector<string> stat_fields;
            stringstream ss(stat_content);
            string field;
            while (ss >> field) {
                stat_fields.push_back(field);
            }

            if (stat_fields.size() > 21) {
                try {
                    unsigned long starttime_ticks = stoull(stat_fields[21]);
                    time_t process_start_time = boot_time + (starttime_ticks / clock_ticks);

                    auto start_tm = *localtime(&process_start_time);
                    char timeStr[50];
                    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M", &start_tm);
                    process.time.startTime = timeStr;

                    time_t current_time = time(nullptr);
                    int duration = static_cast<int>(current_time - process_start_time);

                    int hours = duration / 3600;
                    int minutes = (duration % 3600) / 60;

                    if (hours > 0) {
                        process.time.duration = to_string(hours) + "h " + to_string(minutes) + "m";
                    }
                    else if (minutes > 0) {
                        process.time.duration = to_string(minutes) + "m";
                    }
                    else {
                        process.time.duration = to_string(duration) + "s";
                    }

                    if (stat_fields.size() > 15) {
                        unsigned long utime = stoull(stat_fields[13]);
                        unsigned long stime = stoull(stat_fields[14]);
                        unsigned long total_time = utime + stime;

                        if (duration > 0) {
                            process.cpu = (static_cast<float>(total_time) / clock_ticks / duration) * 100.0;
                        }
                    }
                } catch (const exception& e) {
                    // keep default values
                }
            }
        }

        allProcesses.push_back(process);
    }


    
    ap = allProcesses;

    nbrProcess = ap.size();
    return true;
}

ProcessMonitor::AP ProcessMonitor::getProcess(int index) {
    AP emptyProcess;
    emptyProcess.user = "";
    emptyProcess.cpu = 0.0f;
    emptyProcess.memory = 0.0f;
    emptyProcess.time.startTime = "";
    emptyProcess.time.duration = "";
    emptyProcess.pathName = "";
    
    if (index >= 0 && index < static_cast<int>(ap.size())) {
        return ap[index];
    }
    
    return emptyProcess;
}

string ProcessMonitor::getProcessInfo() {
    std::ostringstream info;
    info << "Number of Processes: " << nbrProcess << "\n";
    // display just 5 process
    for (int i = 0; i < 5 ; i++) {
        AP process = getProcess(i);
        info << "Process " << (i + 1) << ": ";
        info << "  User: " << process.user << " ";
        info << "  CPU Usage: " << fixed << setprecision(2) << process.cpu << "% ";
        info << "  Memory Usage: " << fixed << setprecision(2) << process.memory << "MB ";
        info << "  Start Time: " << process.time.startTime << " ";
        info << "  Duration: " << process.time.duration << " ";
        info << "  Path: " << process.pathName << " \n";
    }
    
    return info.str();
}

int ProcessMonitor::getProcessNbr() {
    return nbrProcess;
}

string ProcessMonitor::getProcessRaw() {
    return "process raw work";
}

string ProcessMonitor::getALLProcessInfo() {
    return "Proc info : " + ProcessMonitor::getProcessInfo() + 
           "\nProc raw: " + ProcessMonitor::getProcessRaw();
}

ProcessMonitor::~ProcessMonitor() {
}