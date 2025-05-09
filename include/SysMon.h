#ifndef SYSMON_H
#define SYSMON_H
using namespace std;
#include <string>
#include <vector>

class ProcessInfo {
public:
    int pid;                
    string name;       
    float cpuUsage;        
    float memoryUsage;
};

class SysMon {
public:
    virtual float getCpuUsage() = 0;                   
    virtual float getMemoryUsage() = 0;               
    virtual vector<ProcessInfo> getProcessList() = 0; 
    virtual ~SysMon() = default;
};

#endif
