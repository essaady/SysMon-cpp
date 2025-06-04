#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <vector>
#include <string>

struct AP
{
    int PID;
    std::string user;
    float cpu;
    float memory;
    std::string pathName;
};

class ProcessMonitor
{
private:
    std::vector<AP> activeProcesses;

public:
    bool update();
    AP getProcess(int index);
    std::string getProcessInfo();
};

#endif
