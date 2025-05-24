#ifndef PROCESSMONITOR_H  
#define PROCESSMONITOR_H

#include <vector>
#include <string>
using namespace std;

struct ProcessInfo {
    int pid;              
    string name;     
    long memory_usage;    
};


class ProcessMonitor {
public:
    vector<ProcessInfo> getAllProcesses();      // Liste ALL the processus
    vector<ProcessInfo> getTopMemoryProcesses(int count);  //  RAM
};

#endif