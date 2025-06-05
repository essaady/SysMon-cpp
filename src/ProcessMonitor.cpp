/*
 * ProcessMonitor.cpp 
    *PROJET : Surveiller les processus système
*CNE:D131430374    
 * NOM ET PRENOM : HASSAN BEN BIROUK
 * Date : *1/6/2025
 * Description : Ce module permet de surveiller l'activité des processus,
 *               leur consommation de ressources et d'effectuer des actions
 */
#include "ProcessMonitor.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

ProcessMonitor::ProcessMonitor() : m_updateInterval(2) {}

void ProcessMonitor::setUpdateInterval(int seconds) {
    if(seconds <= 0) {
        throw invalid_argument("L'intervalle doit être positif");
    }
    m_updateInterval = seconds;
}

vector<ProcessInfo> ProcessMonitor::getRunningProcesses() {
    vector<ProcessInfo> processes;
    vector<pid_t> pids = getAllPids();

    for(const auto& pid : pids) {
        try {
            ProcessInfo info = getProcessInfo(pid);
            processes.push_back(info);
        } catch(const exception& e) {
            cerr << "Erreur lecture PID " << pid << ": " << e.what() << endl;
        }
    }

    sort(processes.begin(), processes.end(), 
        [](const ProcessInfo& a, const ProcessInfo& b) {
            return a.cpuUsage > b.cpuUsage;
        });

    return processes;
}

vector<pid_t> ProcessMonitor::getAllPids() {
    vector<pid_t> pids;
    DIR* dir = opendir("/proc");
    
    if(!dir) {
        throw runtime_error("Impossible d'ouvrir /proc");
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != nullptr) {
        if(entry->d_type == DT_DIR) {
            string name(entry->d_name);
            if(all_of(name.begin(), name.end(), ::isdigit)) {
                pids.push_back(stoi(name));
            }
        }
    }
    closedir(dir);
    return pids;
}

ProcessInfo ProcessMonitor::getProcessInfo(pid_t pid) {
    ProcessInfo info;
    info.pid = pid;
    
    string statPath = "/proc/" + to_string(pid) + "/stat";
    ifstream statFile(statPath);
    if(!statFile) {
        throw runtime_error("Fichier stat inaccessible");
    }

    string line;
    getline(statFile, line);
    istringstream iss(line);
    
    string ignore;
    iss >> ignore >> info.name;
    info.name = info.name.substr(1, info.name.size() - 2);
    
    for(int i = 3; i <= 22; ++i) iss >> ignore;
    iss >> info.utime >> info.stime;
    
    statFile.close();

    string statusPath = "/proc/" + to_string(pid) + "/status";
    ifstream statusFile(statusPath);
    if(statusFile) {
        while(getline(statusFile, line)) {
            if(line.find("VmRSS:") == 0) {
                istringstream(line.substr(6)) >> info.memoryUsage;
                info.memoryUsage /= 1024; // Convertir en Mo
            }
        }
    }

    calculateCpuUsage(info);
    return info;
}

void ProcessMonitor::calculateCpuUsage(ProcessInfo& info) {
    static map<pid_t, pair<unsigned long, unsigned long>> prevTimes;
    
    unsigned long total_time = info.utime + info.stime;
    auto it = prevTimes.find(info.pid);
    
    if(it != prevTimes.end()) {
        unsigned long prev_total = it->second.first + it->second.second;
        unsigned long time_diff = total_time - prev_total;
        info.cpuUsage = (time_diff * 100.0) / (m_updateInterval * sysconf(_SC_CLK_TCK));
    }
    
    prevTimes[info.pid] = {info.utime, info.stime};
}

void ProcessMonitor::exportToCSV(const string& filename) {
    ofstream outfile(filename);
    if(!outfile) {
        throw runtime_error("Impossible d'ouvrir le fichier CSV");
    }

    outfile << "PID,Nom,CPU%,Mémoire(Mo)\n";
    auto processes = getRunningProcesses();
    
    for(const auto& proc : processes) {
        outfile << proc.pid << ","
                << proc.name << ","
                << fixed << setprecision(1) << proc.cpuUsage << ","
                << proc.memoryUsage << "\n";
    }
}

void ProcessMonitor::displayProcesses() const {
    cout << left << setw(8) << "PID" 
         << setw(20) << "Nom" 
         << setw(10) << "CPU%" 
         << setw(15) << "Mémoire(Mo)" 
         << endl;

    for(const auto& proc : m_processes) {
        cout << setw(8) << proc.pid
             << setw(20) << (proc.name.length() > 19 ? proc.name.substr(0, 17) + ".." : proc.name)
             << setw(10) << fixed << setprecision(1) << proc.cpuUsage
             << setw(15) << proc.memoryUsage
             << endl;
    }
}
