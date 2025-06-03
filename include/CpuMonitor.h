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
float getCpuUsage() const;
float getCpuFreq() const;
float getCpuInfo() const;
std::map<std::string,double> CoreUsage() const;
std::string exportText() const;
std::string exportCSV() const;
private:
   struct CPU {
   short nbrCPU;
   float* usagePerCPU;
   float frequency;
   float frequencyMax;
   float usageCPU;
};
std::string rawCPU;
std::map<std::string,CPU> prevCpuData;
std::map<std::string,CPU> currentCpuData;
std::map<std::string,double> cpuUsage;
double calculUsage(const CPU& prev,const CPU& current) const;
bool readCPUdata();
};
#endif
