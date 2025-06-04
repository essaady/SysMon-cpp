#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <string>
using namespace std;

class CpuMonitor {
public:
struct CPU {
    float usageCPU; 
    float frequency; 
    float frequencyMax; 
    float * usagePerCPU; 
} cpu;


short nbrCPU;       
string rawCPU;  

CpuMonitor();

bool update();                 
float getCpuUsage()     
float getCpuFreq()       
string getCpuInfo() 