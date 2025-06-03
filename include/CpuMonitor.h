#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <vector>
#include <string>

class CpuMonitor {
public:
    CpuMonitor();
    bool update();
    float getCpuUsage(); // total CPU usage
    float getCpuFreq();  // current frequency (average)
    float getCpuFreqMax(); // max frequency if available
    short getCoreCount();
    std::vector<float> getPerCoreUsage();
    std::string getCpuInfo(); // summary

private:
    int nbrCPU;
    std::vector<unsigned long long> lastIdleTimes;
    std::vector<unsigned long long> lastTotalTimes;
    std::vector<float> usagePerCPU;
    float frequency;
    float frequencyMax;

    void readStatFile(std::vector<unsigned long long>& idleTimes, std::vector<unsigned long long>& totalTimes);
    float readCpuFrequency();
    float readCpuFrequencyMax();
};

#endif
