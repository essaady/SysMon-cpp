#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <iostream>
#include <vector>


class CpuMonitor{
    

private :
    std::vector <unsigned long> Split(std::string, bool );
    std::vector <unsigned long> Split(std::string, std::string, bool);
    bool ReadStatFile();
    bool ReadFreqFiles();
    void ImplementCpuValues();


    enum States{
        user = 0,
        nice = 1,
        system = 2,
        idle = 3,
        iowait = 4,
        irq = 5,
        softirq = 6,
        steal = 7,
        guest = 8,
        guest_nice = 9
    };

    std::vector<unsigned long> CURR_CPU;
    std::vector<unsigned long> PREV_CPU;
    

    std::vector<std::vector<unsigned long>> CURR_CORES;
    std::vector<std::vector<unsigned long>> PREV_CORES;


    std::vector<double> CURR_CORES_FREQ;
    std::vector<double> MAX_CORES_FREQ;
    
    
public :

    CpuMonitor();
    ~CpuMonitor();


    // frequencies in GHZ
    struct cpu{
        float frequency;
        float frequencyMax;
        float usageCPU;
        float *usagePerCpu;
        short nbrCPU;
    }CPU;

    // informations separated by comma (,)
    std::string rawCPU;

    bool update();
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
    

};

#endif