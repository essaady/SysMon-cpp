#pragma once
#ifndef _CPUMONITOR_H
#define _CPUMONITOR_H


class CpuMon : public SysMon{
public:
    CpuMon(){}
    float calcCpuUsage(int log);
    
private:
    uint64_t getSnap(std::string calc);
    
};
#endif