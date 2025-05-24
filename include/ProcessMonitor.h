#ifndef PROCESSMONITOR_H  
#define PROCESSMONITOR_H

#include <vector>
#include <string>
using namespace std;

// Structure pour stocker les infos d'un processus
struct ProcessInfo {
    int pid;              
    string name;     
    long memory;    
};


class ProcessMonitor {
public:
   vector<ProcessInfo> getAllProcesses();      // Liste of ALL the processus
   vector<ProcessInfo> getTopMemoryProcesses(int count);  //RAM 
};

#endif