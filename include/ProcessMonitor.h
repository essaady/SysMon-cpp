#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
using namespace std;

class ProcessMonitor {
public:
    struct AP {
        string user;
        float cpu;
        float memory;
        string time;
        string pathName;
    };

private:
    AP ap;
    int nbrProcess;

public:
    bool update();
    AP getProcess(int index);
    string getProcessInfo();
};

#endif
