#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H
#include <string>
#include <vector>
#include <stdexcept>

struct Time {
    std::string startTime;
    std::string runTime;
};

struct AP {
    int pid;
    std::string user;
    float cpu;
    float memory;
    std::string pathName;
    Time time;
};

class ProcessMonitor {
public:
    ProcessMonitor();
    bool update();
    AP getProcess(int index) const;
    std::string getProcessInfo() const;

private:
    std::vector<AP> activeProcesses;
};

#endif
