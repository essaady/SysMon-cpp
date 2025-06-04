#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
#include <vector>

struct activeProcesses {
    int pid;
    std::string name;
    float cpu;
    float memory;
    std::string user;
    std::string pathName;
    std::string time;
};

class ProcessMonitor {
    private:
        std::vector<activeProcesses> processes;
    public:
        void update();  

        activeProcesses getProcess(int index);
        std::string getProcessInfo();

};

#endif
