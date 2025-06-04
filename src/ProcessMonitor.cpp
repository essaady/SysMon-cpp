#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H
#include "ProcessMonitor.h"
#include <iostream>

#include <string>
#include <vector>


//process.h
class ProcessMonitor {
private:
    struct AP {
        std::string user;
        float cpu;
        float memory;
        std::string time;
        std::string pathName;
    };

    int* PID;
    int nbrProcess;
    std::vector<AP> processes;

public:
    ProcessMonitor();
    ~ProcessMonitor();

    bool update();                    // Met à jour les informations des processus
    AP getProcess(int index);        // Retourne un processus spécifique
    std::string getProcessInfo();    // Retourne les infos de tous les processus
};

#endif 
//process.cpp
ProcessMonitor::ProcessMonitor() {
    PID = nullptr;
    nbrProcess = 0;
}

ProcessMonitor::~ProcessMonitor() {
    delete PID;
}

bool ProcessMonitor::update() {
    // Nettoyer les anciens processus
    processes.clear();

    // Simulation de deux processus actifs
    AP p1 = {"user1", 12.5f, 100.3f, "00:01:23", "/usr/bin/python3"};
    AP p2 = {"user2", 30.2f, 250.8f, "00:03:45", "/usr/bin/firefox"};

    processes.push_back(p1);
    processes.push_back(p2);
    nbrProcess = processes.size();

    return true;
}

ProcessMonitor::AP ProcessMonitor::getProcess(int index) {
    if (index >= 0 && index < processes.size()) {
        return processes[index];
    }
    return AP();  // Retourne une instance vide si index invalide
}

std::string ProcessMonitor::getProcessInfo() {
    std::string info;
    for (const auto& p : processes) {
        info += "User: " + p.user +
                ", CPU: " + std::to_string(p.cpu) + "%" +
                ", Memory: " + std::to_string(p.memory) + "MB" +
                ", Time: " + p.time +
                ", Path: " + p.pathName + "\n";
    }
    return info;
}
