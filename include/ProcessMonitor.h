#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
#include <vector>

using namespace std;

class ProcessMonitor {
protected:
    int *PID;
    struct AP {
        float cpu ;
        float memory;
        struct {
            string startTime;
            string duration;
        } time;
        string user;
        string pathName;
    };
    vector<AP> ap;
    int nbrProcess;
public:
    ProcessMonitor();
    
    bool update();
    AP getProcess(int index);
    string getProcessInfo();
    string getProcessRaw();
    
    ~ProcessMonitor();
};

#endif