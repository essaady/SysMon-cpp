#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
#include <vector>

class ProcessMonitor {
public:
    struct Time {
        std::string startTime;
        std::string duration;
    };

    struct AP {
        std::string user;
        float cpu;
        float memory;
        Time time;
        std::string pathName;
    };

private:
    int PID;
    std::vector<AP> ap;
    int nbrProcess;

public:
    ProcessMonitor();
    bool update();
    AP getProcess(int index);
    std::string getProcessInfo();
};

#endif // PROCESSMONITOR_H
 
