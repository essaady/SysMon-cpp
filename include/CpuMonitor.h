#ifndef CPUMONITOR_H
#define CPUMONITOR_H
using namespace std;
#include <string>

class CpuMonitor {
public:
    CpuMonitor();
    float getCpuUsage(); 

private:
    unsigned long long lastUser, lastNice, lastSystem, lastIdle;
    void readCpuStats(unsigned long long &user, unsigned long long &nice, unsigned long long &system, unsigned long long &idle);
};

#endif
