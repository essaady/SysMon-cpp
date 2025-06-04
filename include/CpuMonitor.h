#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <string>
#include <map>
//Hamza Tahiri
class CpuMonitor {
public:
    CpuMonitor();
    bool update();
    double totalUsage() const;
    std::map<std::string, double> coreUsage() const;

private:
    struct CpuData {
        unsigned long long user, nice, system, idle, iowait, irq, softirq;
    };

    std::map<std::string, CpuData> prevData;
    std::map<std::string, CpuData> currentData;
    std::map<std::string, double> usage;

    bool readCPUData();
    double calculateUsage(const CpuData& prev, const CpuData& curr) const;
};

#endif
