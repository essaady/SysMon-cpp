#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H
#include<string>
#include<vector>
#include<map>
class CpuMonitor{
public:
CpuMonitor();
~CpuMonitor();
bool update();
double totalUsage() const;
std::map<std::string,double> CoreUsage() const;
std::string exportText() const;
std::string exportCSV() const;
private:
   struct CPUdata {
    unsigned user,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
};
std::map<std::string,CPUdata> prevCpuData;
std::map<std::string,CPUdata> currentCpuData;
std::map<std::string,double> cpuUsage;
double calculUsage(const CPUdata& prev,const CPUdata& current) const;
bool readCPUdata();
};
#endif
