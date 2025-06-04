#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
#include <vector>

class ProcessMonitor {
public:
    struct activeProcesses {
        std::string user;
        float cpu;
        float memory;
        std::string pathName;
        std::string time;
    };

    bool update();
    activeProcesses getProcess(int index);
    std::string getProcessInfo();
    std::string getProcessRaw();

private:
    std::vector<activeProcesses> AP;
    int nbrProcess;
};

#endif
