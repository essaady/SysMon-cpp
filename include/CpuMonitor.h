#pragma once
#ifndef _CPU_MON_H
#define _CPU_MON_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>

class CpuMon{
public:
    CpuMon(){}
    std::string getCpuInfo();
    float calcCpuUsage();
    
private:

    std::string buffer;
    uint64_t getSnap(std::string calc);


};

#endif