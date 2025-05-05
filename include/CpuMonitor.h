#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;



class CpuMonitor{
    
    private :
    static void ReadStatFile();

    public :
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

    static vector<unsigned long> CPU;
    static vector<vector<unsigned long>> CORES;
    static unsigned int CORES_NUMBER;
    static unsigned long INTERRUPTS;
    static unsigned long CONTEXT_SWITCHES;
    static unsigned long BOOT_TIME;
    static unsigned long PROCESSES;
    static unsigned long PROCESSES_RUNNING;
    static unsigned long PROCESSES_BLOCKED;
    static vector<unsigned long> SOFT_IRQS;



    static void RefreshCpuMonitor();
    

};

#endif