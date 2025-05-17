#include "../include/ProcessMonitor.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <dirent.h>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

ProcessMonitor::ProcessMonitor() : systemUptime(0), hertz(sysconf(_SC_CLK_TCK)) {
    // Initialisation
    update();
}

ProcessMonitor::~ProcessMonitor() {
    // Nettoyage si nécessaire
}

bool ProcessMonitor::update() {
    // Mettre à jour l'uptime du système
    systemUptime = readSystemUptime();
    
    // Vider la liste des processus
    processes.clear();
    
    // Ouvrir le répertoire /proc
    DIR* dir = opendir("/proc");
    if (!dir) {
        std::cerr << "Failed to open /proc directory" << std::endl;
        return false;
    }
    
    // Parcourir les entrées du répertoire
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Vérifier si l'entrée est un répertoire et si son nom est un nombre (PID)
        if (entry->d_type == DT_DIR) {
            char* endptr;
            int pid = strtol(entry->d_name, &endptr, 10);
            if (*endptr == '\0') {
                // C'est un PID, lire les informations du processus
                ProcessInfo info = readProcessInfo(pid);
                processes.push_back(info);
            }
        }
    }
    
    closedir(dir);
    
    // Trier les processus par utilisation CPU (décroissant)
    std::sort(processes.begin(), processes.end(), 
              [](const ProcessInfo& a, const ProcessInfo& b) {
                  return a.cpuUsage > b.cpuUsage;
              });
    
    return true;
}

std::vector<ProcessInfo> ProcessMonitor::getProcesses() const {
    return processes;
}

std::vector<ProcessInfo> ProcessMonitor::getTopCPUProcesses(int n) const {
    if (n <= 0 || n >= static_cast<int>(processes.size())) {
        return processes;
    }
    
    std::vector<ProcessInfo> topProcesses(processes.begin(), processes.begin() + n);
    return topProcesses;
}

std::vector<ProcessInfo> ProcessMonitor::getTopMemoryProcesses(int n) const {
    if (n <= 0 || n >= static_cast<int>(processes.size())) {
        return processes;
    }
    
    // Copier les processus et trier par utilisation mémoire
    std::vector<ProcessInfo> memProcesses = processes;
    std::sort(memProcesses.begin(), memProcesses.end(), 
              [](const ProcessInfo& a, const ProcessInfo& b) {
                  return a.memoryUsage > b.memoryUsage;
              });
    
    std::vector<ProcessInfo> topProcesses(memProcesses.begin(), memProcesses.begin() + n);
    return topProcesses;
}

ProcessInfo ProcessMonitor::getProcessInfo(int pid) const {
    for (const auto& process : processes) {
        if (process.pid == pid) {
            return process;
        }
    }
    
    // Retourner un processus vide si non trouvé
    return ProcessInfo{};
}

std::string ProcessMonitor::exportAsText(int limit) const {
    std::ostringstream oss;
    
    oss << "Processes: " << processes.size() << " total\n";
    oss << std::setw(7) << "PID" << " | " 
        << std::setw(15) << "USER" << " | " 
        << std::setw(8) << "CPU%" << " | " 
        << std::setw(8) << "MEM%" << " | " 
        << std::setw(8) << "RSS(MB)" << " | " 
        << std::setw(8) << "VSZ(MB)" << " | " 
        << std::setw(5) << "PRI" << " | " 
        << std::setw(5) << "NI" << " | " 
        << std::setw(5) << "THR" << " | " 
        << std::setw(5) << "S" << " | " 
        << "COMMAND\n";
    
    oss << std::setw(5) << "THR" << " | "
        << std::setw(5) << "S" << " | "
        << "COMMAND\n";

    
    // Limiter le nombre de processus à afficher si demandé
    int count = 0;
    for (const auto& process : processes) {
        if (limit > 0 && count >= limit) {
            break;
        }
        
        // Convertir RSS et VSZ en MB
        double rssMB = process.rss / 1024.0;
        double vszMB = process.vsz / 1024.0;
        
        oss << std::setw(7) << process.pid << " | " 
            << std::setw(15) << process.user << " | " 
            << std::setw(7) << std::fixed << std::setprecision(1) << process.cpuUsage << "% | " 
            << std::setw(7) << std::fixed << std::setprecision(1) << process.memoryUsage << "% | " 
            << std::setw(8) << std::fixed << std::setprecision(1) << rssMB << " | " 
            << std::setw(8) << std::fixed << std::setprecision(1) << vszMB << " | " 
            << std::setw(5) << process.priority << " | " 
            << std::setw(5) << process.nice << " | " 
            << std::setw(5) << process.threads << " | " 
            << std::setw(5) << process.state << " | " 
            << process.name << "\n";
        
        count++;
    }
    
    return oss.str();
}

std::string ProcessMonitor::exportAsCSV(int limit) const {
    std::ostringstream oss;
    
    // En-tête
    oss << "PID,USER,CPU%,MEM%,RSS(MB),VSZ(MB),PRI,NI,THR,STATE,COMMAND\n";
    
    // Limiter le nombre de processus à afficher si demandé
    int count = 0;
    for (const auto& process : processes) {
        if (limit > 0 && count >= limit) {
            break;
        }
        
        // Convertir RSS et VSZ en MB
        double rssMB = process.rss / 1024.0;
        double vszMB = process.vsz / 1024.0;
        
        oss << process.pid << ","
            << process.user << ","
            << std::fixed << std::setprecision(1) << process.cpuUsage << ","
            << std::fixed << std::setprecision(1) << process.memoryUsage << ","
            << std::fixed << std::setprecision(1) << rssMB << ","
            << std::fixed << std::setprecision(1) << vszMB << ","
            << process.priority << ","
            << process.nice << ","
            << process.threads << ","
            << process.state << ","
            << process.name << "\n";
        
        count++;
    }
    
    return oss.str();
}

ProcessInfo ProcessMonitor::readProcessInfo(int pid) {
    ProcessInfo info;
    info.pid = pid;
    
    // Lire le statut du processus
    std::string statPath = "/proc/" + std::to_string(pid) + "/stat";
    std::ifstream statFile(statPath);
    unsigned long long totalTime = 0;  // initialisé à 0 par défaut
    if (statFile.is_open()) {
        std::string line;
        if (std::getline(statFile, line)) {
            std::istringstream iss(line);
            
            // Format: pid (comm) state ppid ...
            int dummy;
            char state;
            unsigned long long utime, stime, starttime;
            unsigned long long vsize, rss;
            int priority, nice, threads;
            
            // Extraire le nom entre parenthèses
            size_t start = line.find('(');
            size_t end = line.rfind(')');
            if (start != std::string::npos && end != std::string::npos) {
                info.name = line.substr(start + 1, end - start - 1);
                
                // Recréer un flux pour lire le reste des données
                std::string restOfLine = line.substr(end + 2); // +2 pour sauter ) et l'espace
                std::istringstream restIss(restOfLine);
                
                restIss >> state >> dummy >> dummy >> dummy >> dummy >> dummy >> dummy
                       >> dummy >> dummy >> dummy >> dummy >> utime >> stime >> dummy
                       >> dummy >> priority >> nice >> threads >> dummy >> starttime
                       >> vsize >> rss;
                unsigned long long totalTime = utime + stime;
                info.state = state;
                info.priority = priority;
                info.nice = nice;
                info.threads = threads;
                info.startTime = starttime;
                info.vsz = vsize / 1024; // Convertir en kB
                info.rss = rss * (sysconf(_SC_PAGESIZE) / 1024); // Convertir en kB
            }
        }
        statFile.close();
    }
    
    // Lire le statut du processus pour obtenir l'UID
    std::string statusPath = "/proc/" + std::to_string(pid) + "/status";
    std::ifstream statusFile(statusPath);
    if (statusFile.is_open()) {
        std::string line;
        while (std::getline(statusFile, line)) {
            if (line.substr(0, 4) == "Uid:") {
                std::istringstream iss(line);
                std::string dummy;
                uid_t uid;
                iss >> dummy >> uid;
                info.user = getUsernameFromUid(uid);
                break;
            }
        }
        statusFile.close();
    }
    
    // Calculer l'utilisation CPU

    info.cpuUsage = calculateProcessCpuUsage(pid, totalTime);
    
    // Calculer l'utilisation mémoire (pourcentage de la mémoire physique)
    // Lire la mémoire totale du système
    std::ifstream memFile("/proc/meminfo");
    if (memFile.is_open()) {
        std::string line;
        while (std::getline(memFile, line)) {
            if (line.substr(0, 9) == "MemTotal:") {
                std::istringstream iss(line);
                std::string dummy;
                unsigned long long totalMem;
                iss >> dummy >> totalMem;
                
                // Calculer le pourcentage
                if (totalMem > 0) {
                    info.memoryUsage = 100.0 * info.rss / totalMem;
                }
                break;
            }
        }
        memFile.close();
    }
    
    return info;
}

std::string ProcessMonitor::getUsernameFromUid(uid_t uid) const {
    struct passwd* pw = getpwuid(uid);
    if (pw) {
        return pw->pw_name;
    }
    return std::to_string(uid);
}

unsigned long long ProcessMonitor::readSystemUptime() const {
    std::ifstream uptimeFile("/proc/uptime");
    if (!uptimeFile.is_open()) {
        std::cerr << "Failed to open /proc/uptime" << std::endl;
        return 0;
    }
    
    double uptime;
    uptimeFile >> uptime;
    uptimeFile.close();
    
    return static_cast<unsigned long long>(uptime * hertz);
}

double ProcessMonitor::calculateProcessCpuUsage(int pid, unsigned long long currentTime) {
    double cpuUsage = 0.0;
    
    // Vérifier si nous avons des données précédentes pour ce processus
    auto it = prevCpuTimes.find(pid);
    if (it != prevCpuTimes.end()) {
        unsigned long long prevTime = it->second;
        unsigned long long timeDiff = currentTime - prevTime;
        
        // Calculer le pourcentage d'utilisation CPU
        // (temps CPU utilisé / temps écoulé) * 100
        double elapsedTime = static_cast<double>(systemUptime);
        if (elapsedTime > 0) {
            cpuUsage = 100.0 * timeDiff / (hertz * elapsedTime);
        }
    }
    
    // Mettre à jour les données précédentes
    prevCpuTimes[pid] = currentTime;
    
    return cpuUsage;
}

