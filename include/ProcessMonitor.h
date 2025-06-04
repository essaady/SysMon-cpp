#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
#include <vector>
#include <ctime>

using namespace std;

struct ProcessTime {
    time_t startTime;
    time_t runTime;
    string startTimeStr;
    string runTimeStr;
};

struct AP {
    int PID;
    string user;
    float cpu;
    float memory;
    ProcessTime processTime;
    string pathName;
    string processName;
    string status;
};

class ProcessMonitor {
private:
    int PID;
    struct AP ap;
    vector<AP> processList;
    int nbrProcess;
    string user;
    float cpu;
    float memory;
    ProcessTime time;
    string pathName;

public:
    ProcessMonitor();
    ~ProcessMonitor();
    
    bool update();
    AP getProcess(int index);
    string getProcessInfo();
    
    int getNbrProcess() const { return nbrProcess; }
    vector<AP> getProcessList() const { return processList; }
    AP getProcessByPID(int pid);
    vector<AP> getProcessesByUser(const string& userName);
    vector<AP> getTopCpuProcesses(int count = 10);
    vector<AP> getTopMemoryProcesses(int count = 10);
};

#endif


