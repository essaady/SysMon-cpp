#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <vector>

class Process {
public:
    struct ActiveProcess {
        float cpu;
        float memory;
        std::string time;
        std::string user;
        std::string pathName;
        int pid;
    };

    Process();
    ~Process();

    bool update();
    ActiveProcess getProcess(int index) const;
    std::string getProcessInfo() const;
    std::string getProcessRaw() const;

private:
    std::vector<ActiveProcess> processes;
    int nbrProcess;

    std::string getUserFromUID(uid_t uid) const;
    float getCPUUsage(int pid) const;
    float getMemoryUsage(int pid) const;
    std::string getProcessTime(int pid) const;
    std::string getPathName(int pid) const;
};

#endif
