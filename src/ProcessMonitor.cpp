#include "ProcessMonitor.h"
#include <iostream>
#include <sstream>

ProcessMonitor::ProcessMonitor() {
    PID = 0;
    nbrProcess = 0;
}


bool ProcessMonitor::update() {
    ap.clear();

    for (int i = 0; i < 3; i++) {
        AP process;
        process.cpu = (float)(55); 
        process.memory = (float)(500);
        process.time.startTime = "2025-06-04 10:11";
        process.time.duration = "12h 13m";
        process.user = "user111";
        process.pathName = "/proc/2121";
        
        ap.push_back(process);
    }

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
    
    for (int i = 0; i < nbrProcess; i++) {
        AP process = getProcess(i);
        info << "Process " << (i + 1) << ":\n";
        info << "  User: " << process.user << "\n";
        info << "  CPU Usage: " << process.cpu << "%\n";
        info << "  Memory Usage: " << process.memory << "MB\n";
        info << "  Start Time: " << process.time.startTime << "\n";
        info << "  Duration: " << process.time.duration << "\n";
        info << "  Path: " << process.pathName << "\n\n";
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

ProcessMonitor::~ProcessMonitor() {}
