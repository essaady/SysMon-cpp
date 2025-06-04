#include "ProcessMonitor.h"
#include <iostream>

ProcessMonitor::ProcessMonitor() {
    PID = 0;
    nbrProcess = 0;
}


bool ProcessMonitor::update() {
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
    return "process info work";
}

string ProcessMonitor::getProcessRaw() {
    return "process raw work";
}

ProcessMonitor::~ProcessMonitor() {}
