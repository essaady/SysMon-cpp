#include "ProcessMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <algorithm>
#include <pwd.h>
#include <unistd.h>
#include <iomanip>

using namespace std;

ProcessMonitor::ProcessMonitor() {
    PID = 0;
    nbrProcess = 0;
    user = "";
    cpu = 0.0f;
    memory = 0.0f;
    pathName = "";

    ap.PID = 0;
    ap.user = "";
    ap.cpu = 0.0f;
    ap.memory = 0.0f;
    ap.pathName = "";
    ap.processName = "";
    ap.status = "";
}

ProcessMonitor::~ProcessMonitor() {}

bool ProcessMonitor::update() {
    try {
        processList.clear();
        nbrProcess = 0;
        
        DIR* procDir = opendir("/proc");
        if (!procDir) {
            cerr << "Erreur: Impossible d'ouvrir /proc" << endl;
            return false;
        }
        
        struct dirent* entry;
        while ((entry = readdir(procDir)) != nullptr) {

            string dirname = entry->d_name;
            if (dirname.find_first_not_of("0123456789") != string::npos) {
                continue;
            }
            
            int pid = stoi(dirname);
            AP process;
            
            if (getProcessDetails(pid, process)) {
                processList.push_back(process);
                nbrProcess++;
            }
        }
        
        closedir(procDir);
        sort(processList.begin(), processList.end(), 
             [](const AP& a, const AP& b) { return a.cpu > b.cpu; });
        
        return true;
    } catch (const exception& e) {
        cerr << "Erreur lors de la mise à jour des processus: " << e.what() << endl;
        return false;
    }
}

bool ProcessMonitor::getProcessDetails(int pid, AP& process) {

bool ProcessMonitor::getProcessDetails(int pid, AP& process) {
    try {

        string statPath = "/proc/" + to_string(pid) + "/stat";
        ifstream statFile(statPath);
        if (!statFile.is_open()) {
            return false;
        }
        
        string line;
        getline(statFile, line);
        statFile.close();
        
        istringstream iss(line);
        string token;
        vector<string> tokens;

        while (iss >> token) {
            tokens.push_back(token);
        }
        
        if (tokens.size() < 24) {
            return false;
        }
        
        process.PID = pid;
        process.processName = tokens[1];
        if (process.processName.front() == '(' && process.processName.back() == ')') {
            process.processName = process.processName.substr(1, process.processName.length() - 2);
        }
        
        process.status = tokens[2];
        unsigned long utime = stoul(tokens[13]);
        unsigned long stime = stoul(tokens[14]);
        unsigned long total_time = utime + stime;
        process.cpu = static_cast<float>(total_time) / 100.0f; 
        
        string statusPath = "/proc/" + to_string(pid) + "/status";
        ifstream statusFile(statusPath);
        if (statusFile.is_open()) {
            string statusLine;
            while (getline(statusFile, statusLine)) {
                if (statusLine.substr(0, 6) == "VmRSS:") {
                    istringstream statusIss(statusLine);
                    string key, value, unit;
                    statusIss >> key >> value >> unit;
                    process.memory = stof(value) / 1024.0f; 
                    break;
                }
            }
            statusFile.close();
        }
        
        string uidPath = "/proc/" + to_string(pid) + "/loginuid";
        ifstream uidFile(uidPath);
        if (uidFile.is_open()) {
            string uidStr;
            getline(uidFile, uidStr);
            if (!uidStr.empty() && uidStr != "-1") {
                int uid = stoi(uidStr);
                struct passwd* pw = getpwuid(uid);
                if (pw) {
                    process.user = pw->pw_name;
                }
            }
            uidFile.close();
        }
        
        if (process.user.empty()) {
            process.user = "unknown";
        }
        
        string exePath = "/proc/" + to_string(pid) + "/exe";
        char pathBuffer[1024];
        ssize_t len = readlink(exePath.c_str(), pathBuffer, sizeof(pathBuffer) - 1);
        if (len != -1) {
            pathBuffer[len] = '\0';
            process.pathName = pathBuffer;
        } else {
            process.pathName = "N/A";
        }
        
        process.processTime.startTime = time(nullptr); 
        process.processTime.runTime = 0; 
        process.processTime.startTimeStr = "N/A";
        process.processTime.runTimeStr = "N/A";
        
        return true;
    } catch (const exception& e) {
        return false;
    }
}

AP ProcessMonitor::getProcess(int index) {
    if (index >= 0 && index < processList.size()) {
        return processList[index];
    }
    return AP(); 
}

string ProcessMonitor::getProcessInfo() {
    stringstream ss;
    ss << "Top 10 des processus par utilisation CPU:" << endl;
    ss << "PID\tNom\t\tUtilisateur\tCPU\tMémoire(MB)\tChemin" << endl;
    ss << "---\t---\t\t-----------\t---\t-----------\t------" << endl;
    
    int count = min(10, static_cast<int>(processList.size()));
    for (int i = 0; i < count; i++) {
        const AP& proc = processList[i];
        ss << proc.PID << "\t";
        ss << proc.processName.substr(0, 15) << "\t";
        ss << proc.user.substr(0, 10) << "\t";
        ss << fixed << setprecision(1) << proc.cpu << "\t";
        ss << fixed << setprecision(1) << proc.memory << "\t\t";
        ss << proc.pathName << endl;
    }
    
    return ss.str();
}

AP ProcessMonitor::getProcessByPID(int pid) {
    for (const auto& process : processList) {
        if (process.PID == pid) {
            return process;
        }
    }
    return AP(); 
}

vector<AP> ProcessMonitor::getProcessesByUser(const string& userName) {
    vector<AP> userProcesses;
    for (const auto& process : processList) {
        if (process.user == userName) {
            userProcesses.push_back(process);
        }
    }
    return userProcesses;
}

vector<AP> ProcessMonitor::getTopCpuProcesses(int count) {
    vector<AP> topProcesses = processList;
    sort(topProcesses.begin(), topProcesses.end(), 
         [](const AP& a, const AP& b) { return a.cpu > b.cpu; });
    
    if (count < topProcesses.size()) {
        topProcesses.resize(count);
    }
    return topProcesses;
}

vector<AP> ProcessMonitor::getTopMemoryProcesses(int count) {
    vector<AP> topProcesses = processList;
    sort(topProcesses.begin(), topProcesses.end(), 
         [](const AP& a, const AP& b) { return a.memory > b.memory; });
    
    if (count < topProcesses.size()) {
        topProcesses.resize(count);
    }
    return topProcesses;
}
