#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <vector>
#include <string>

class CpuMonitor {
public:
   /
    struct CPU {
        short nbrCPU;                      
        std::vector<float> usagePerCPU;    
        float frequency;                   
        float frequencyMax;                
    };

private:
    CPU cpu; 

public:
    CpuMonitor();                          
    bool update();                        
    float getCpuUsage();                   
    float getCpuFreq();                    
    std::string getCpuInfo();              
};

#endif 
