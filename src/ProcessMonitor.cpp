#include "ProcessMonitor.h"
#include <algorithm>  
#include <fstream>    
#include <dirent.h>  
using namespace std; 

vector<ProcessInfo> ProcessMonitor::getAllProcesses() {
    vector<ProcessInfo> processes;
    DIR* procDir = opendir("/proc");  // open file /proc

    dirent* entry;
    while (entry = readdir(procDir)) {  
       
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            int pid = stoi(entry->d_name);

            // Read the file /proc/[PID]/stat
            ifstream statFile("/proc" + to_string(pid) + "/stat");
            if (!statFile) continue;  // if erreur then pass 

            ProcessInfo proc;
            proc.pid = pid;
            statFile >> proc.pid >> proc.name;  

           // "Real Memory Usage"
            long rss;
            for (int i = 0; i < 22; i++) ;
            proc.memory_usage = rss * 4;  //  Ko

            processes.push_back(proc);
        }
    }
    closedir(procDir);
    return processes;
}

vector<ProcessInfo> ProcessMonitor::getTopMemoryProcesses(int count) {
    auto processes = getAllProcesses();  // ALL process

    // "Processes consuming the most memory"
    sort(processes.begin(), processes.end(),(const ProcessInfo& a, const ProcessInfo& b) {
            return a.memory_usage > b.memory_usage;
        });

    if (processes.size() > count) {
        processes.resize(count);
    }
    return processes;
}