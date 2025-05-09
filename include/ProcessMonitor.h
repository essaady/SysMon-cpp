#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H
using namespace std;
#include <string>
#include <vector>

struct ProcessInfo {
    int pid;
    string name;
    float cpuUsage;
    float memoryUsage;
};
class ProcessMonitor {
public:
    ProcessMonitor();
    vector<ProcessInfo> getProcessList();

private:
    unsigned long getTotalMemory();
    float getCpuUsageForProcess(int pid);
    float getMemoryUsageForProcess(int pid, unsigned long memTotal);
};

#endif
