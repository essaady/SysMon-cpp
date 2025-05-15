#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <string>
#include <vector>
#include <map>
#include <sys/types.h>

/**
 * @class ProcessInfo
 * @brief Structure contenant les informations d'un processus
 */
struct ProcessInfo {
    int pid;
    std::string name;
    std::string user;
    std::string state;
    double cpuUsage;
    double memoryUsage;
    unsigned long long rss;
    unsigned long long vsz;
    unsigned int priority;
    unsigned int nice;
    unsigned long long startTime;
    unsigned int threads;
};

/**
 * @class ProcessMonitor
 * @brief Classe pour surveiller les processus actifs
 */
class ProcessMonitor {
public:
    ProcessMonitor();
    ~ProcessMonitor();

    bool update();

    std::vector<ProcessInfo> getProcesses() const;
    std::vector<ProcessInfo> getTopCPUProcesses(int n) const;
    std::vector<ProcessInfo> getTopMemoryProcesses(int n) const;
    ProcessInfo getProcessInfo(int pid) const;

    std::string exportAsText(int limit = 0) const;
    std::string exportAsCSV(int limit = 0) const;

private:
    std::vector<ProcessInfo> processes;
    std::map<int, unsigned long long> prevCpuTimes;
    unsigned long long prevSystemUptime;
    unsigned long long systemUptime;
    unsigned long long hertz;

    ProcessInfo readProcessInfo(int pid);
    std::string getUsernameFromUid(uid_t uid) const;
    unsigned long long readSystemUptime() const;
    double calculateProcessCpuUsage(int pid, unsigned long long currentTime, unsigned long long elapsedTime);
};

#endif // PROCESS_MONITOR_H
