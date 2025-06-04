// src/ProcessMonitor.cpp
#include "ProcessMonitor.h"
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <pwd.h>
#include <unistd.h>

bool ProcessMonitor::update() {
    DIR* proc = opendir("/proc");
    if (!proc) return false;

    nbrProcess = 0;

    struct dirent* entry;
    while ((entry = readdir(proc)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            std::string pidDir = entry->d_name;
            if (std::all_of(pidDir.begin(), pidDir.end(), ::isdigit)) {
                nbrProcess++;
                
            }
        }
    }

    closedir(proc);
    return true;
}

AP ProcessMonitor::getProcess(int index) {
    return ap; 
}

std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream oss;
    oss << "Nombre de processus actifs : " << nbrProcess;
    return oss.str();
}
