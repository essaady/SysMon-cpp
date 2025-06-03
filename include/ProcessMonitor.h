#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
#include <vector>

struct AP {
    std::string user;
    int pid;
    float cpu;
    float memory;
    std::string time;
    std::string pathName;
};

class ProcessMonitor {
public:
    ProcessMonitor();
    bool update();
    AP getProcess(int index);
    std::string getProcessInfo();
    int getProcessCount();

private:
    std::vector<AP> processes;
    int totalMemoryMB;
    long clockTicks;

    std::string getUserFromUID(int uid);
    float getMemoryUsage(int pid);
    float getCpuUsage(int pid);
    std::string getTimeFromTicks(long ticks);
};

#endif
