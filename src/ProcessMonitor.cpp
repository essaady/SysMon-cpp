#include "../include/ProcessMonitor.h"

#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <processthreadsapi.h>
#include <string>

ProcessMonitor::ProcessMonitor() : totalSystemTime(0), prevTotalSystemTime(0) {
    update(); // Initialisation avec les valeurs actuelles
}

void ProcessMonitor::update() {
    // Met à jour les statistiques système
    prevTotalSystemTime = totalSystemTime;
    
    // Lit le temps CPU total du système
    std::ifstream statFile("/proc/stat");
    if (statFile.is_open()) {
        std::string line;
        if (std::getline(statFile, line) && line.substr(0, 3) == "cpu") {
            std::istringstream iss(line);
            std::string cpu;
            long user, nice, system, idle, iowait, irq, softirq, steal;
            
            iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
            totalSystemTime = user + nice + system + idle + iowait + irq + softirq + steal;
        }
        statFile.close();
    }
    
    // Sauvegarde les temps CPU précédents
    for (const auto& proc : processes) {
        prevCpuTimes[proc.pid] = proc.cpuUsage;
    }
    
    // Vide la liste actuelle et lit les nouveaux processus
    processes.clear();
    readProcessList();
    
    // Trie les processus par utilisation CPU (décroissant)
    std::sort(processes.begin(), processes.end(), 
              [](const ProcessInfo& a, const ProcessInfo& b) {
                  return a.cpuUsage > b.cpuUsage;
              });
}

void ProcessMonitor::readProcessList() {
    DIR* dir = opendir("/proc");
    if (!dir) {
        std::cerr << "Erreur: Impossible d'ouvrir /proc" << std::endl;
        return;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Vérifie si c'est un dossier et s'il s'agit d'un PID (nombre)
        if (entry->d_type == DT_DIR) {
            char* endptr;
            long pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0') {  // Le nom est un nombre entier valide
                // Chemin du fichier stat du processus
                std::string statPath = "/proc/" + std::string(entry->d_name) + "/stat";
                std::ifstream statFile(statPath);
                
                if (statFile.is_open()) {
                    std::string line;
                    if (std::getline(statFile, line)) {
                        ProcessInfo proc;
                        proc.pid = pid;
                        
                        // Parse le fichier stat
                        size_t comm_start = line.find('(');
                        size_t comm_end = line.find(')', comm_start);
                        
                        if (comm_start != std::string::npos && comm_end != std::string::npos) {
                            // Extrait le nom du processus entre parenthèses
                            proc.name = line.substr(comm_start + 1, comm_end - comm_start - 1);
                            
                            // Parse le reste de la ligne
                            std::string restOfLine = line.substr(comm_end + 2);  // +2 pour sauter la parenthèse et l'espace
                            std::istringstream iss(restOfLine);
                            
                            // Le premier champ après la parenthèse est l'état
                            iss >> proc.state;
                            
                            // Avance jusqu'aux champs utime et stime (positions 14 et 15 dans le reste de la ligne)
                            std::string tmp;
                            for (int i = 0; i < 11; ++i) {  // Saute 11 champs
                                iss >> tmp;
                            }
                            
                            unsigned long utime, stime;
                            iss >> utime >> stime;
                            
                            // Calcule l'utilisation CPU
                            double total_time = utime + stime;
                            
                            // Calcul le pourcentage d'utilisation CPU
                            if (prevCpuTimes.find(pid) != prevCpuTimes.end() && totalSystemTime > prevTotalSystemTime) {
                                double prevTime = prevCpuTimes[pid];
                                proc.cpuUsage = calculateCpuUsage(pid, prevTime);
                            } else {
                                proc.cpuUsage = 0.0;
                            }
                            prevCpuTimes[pid] = total_time;
                        }
                    }
                    statFile.close();
                    
                    // Lit les informations mémoire du processus
                    std::string statusPath = "/proc/" + std::string(entry->d_name) + "/status";
                    std::ifstream statusFile(statusPath);
                    
                    if (statusFile.is_open()) {
                        std::string line;
                        while (std::getline(statusFile, line)) {
                            if (line.substr(0, 6) == "VmRSS:") {
                                std::istringstream iss(line.substr(6));
                                iss >> proc.memoryRss;
                                break;
                            }
                        }
                        statusFile.close();
                    }
                    
                    // Calcule le pourcentage d'utilisation mémoire
                    std::ifstream memInfoFile("/proc/meminfo");
                    if (memInfoFile.is_open()) {
                        std::string line;
                        long totalMem = 0;
                        while (std::getline(memInfoFile, line)) {
                            if (line.substr(0, 9) == "MemTotal:") {
                                std::istringstream iss(line.substr(9));
                                iss >> totalMem;
                                break;
                            }
                        }
                        memInfoFile.close();
                        
                        if (totalMem > 0) {
                            proc.memoryUsage = (static_cast<double>(proc.memoryRss) / totalMem) * 100.0;
                        } else {
                            proc.memoryUsage = 0.0;
                        }
                    }
                    
                    // Obtient l'utilisateur propriétaire du processus
                    struct stat st;
                    if (stat(("/proc/" + std::string(entry->d_name)).c_str(), &st) == 0) {
                        struct passwd* pw = getpwuid(st.st_uid);
                        if (pw) {
                            proc.user = pw->pw_name;
                        } else {
                            proc.user = std::to_string(st.st_uid);
                        }
                    } else {
                        proc.user = "unknown";
                    }
                    
                    // Ajoute le processus à la liste
                    processes.push_back(proc);
                }
            }
        }
    }
    
    closedir(dir);
}

double ProcessMonitor::calculateCpuUsage(int pid, double prevTime) {
    // Lit le temps CPU actuel du processus
    double currentTime = 0.0;
    std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream statFile(statPath);
    
    if (statFile.is_open()) {
        std::string line;
        if (std::getline(statFile, line)) {
            size_t comm_end = line.find(')', line.find('('));
            if (comm_end != std::string::npos) {
                std::string restOfLine = line.substr(comm_end + 2);
                std::istringstream iss(restOfLine);
                
                // Saute les champs jusqu'à utime et stime
                std::string tmp;
                for (int i = 0; i < 11; ++i) {
                    iss >> tmp;
                }
                
                unsigned long utime, stime;
                iss >> utime >> stime;
                currentTime = utime + stime;
            }
        }
        statFile.close();
    }
    
    // Calcule le pourcentage d'utilisation CPU
    double timeDiff = currentTime - prevTime;
    double systemTimeDiff = totalSystemTime - prevTotalSystemTime;
    
    if (systemTimeDiff > 0) {
        return (timeDiff / systemTimeDiff) * 100.0;
    }
    
    return 0.0;
}

std::vector<ProcessInfo> ProcessMonitor::getTopProcesses(int count) const {
    std::vector<ProcessInfo> result;
    int actualCount = std::min(count, static_cast<int>(processes.size()));
    
    result.assign(processes.begin(), processes.begin() + actualCount);
    return result;

}