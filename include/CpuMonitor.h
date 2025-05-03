#pragma once
#ifndef _CPU_MON_H
#define _CPU_MON_H


#include "./SysMon.h"


class CpuMon : public SysMon{
public:
    CpuMon(){}
    float calcCpuUsage(int log);
    
private:
    uint64_t getSnap(std::string calc);
    
};
#endif