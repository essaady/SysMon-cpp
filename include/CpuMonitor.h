#ifndef CPU_H
#define CPU_H

#include <string>

class Cpu {
public:
    struct Info {
        float frequency;       
        float frequencyMax;     
        float usage;           
        float* usagePerCore;    
        short coreCount;        
    };

private:
    Info cpu;
    std::string rawInfo;      

public:
    Cpu();
    ~Cpu();

    bool update();
    
    float getUsage() const { return cpu.usage; }
    float getFrequency() const { return cpu.frequency; }
    std::string getRawInfo() const { return rawInfo; }
    const Info& getInfo() const { return cpu; }
};

#endif 
