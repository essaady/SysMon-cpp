#include "../include/ProcessMonitor.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <dirent.h>
#include <unistd.h>
#include <unordered_map>

namespace {
    bool getProcessInfo(int pid, ProcessInfo& info, long& prev_total_time) {
        
    }
}

std::vector<ProcessInfo> ProcessMonitor::getTopProcesses(int count) {
    std::vector<ProcessInfo> processes;
    DIR* proc_dir = opendir("/proc");
    
    if (!proc_dir) {
        return processes;
    }

    // ... (rest of the implementation same as before, 
    // but remove the main() function and return the sorted vector)
    
    // Instead of printing, just return the top 'count' processes
    if (processes.size() > static_cast<size_t>(count)) {
        processes.resize(count);
    }
    
    return processes;
}