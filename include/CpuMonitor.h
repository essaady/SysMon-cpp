#ifndef CPUMONITOR_H
#define CPUMONITOR_H

#include <iostream>
#include <vector>


using namespace std;

///////////////////////////////////////////////////////////////////////////
//////////////////////                     ////////////////////////////////
//////////////////////  IMPORTANT TO READ  ////////////////////////////////
//////////////////////                     ////////////////////////////////
///////////////////////////////////////////////////////////////////////////
/*
First of all you have to understand the structure of /proc/stat file.

All what I have done is to collect the information from the file 
and store them in variables, so we can display them or calculate
other values that depends on them.

All the members and methods in this class are static, so you don't
have to create an object of that class.

The first thing to do is to call the CpuMonitor::RefreshCpuMonitor() 
methode, so it reads the file and also call it whenever you need to 
read the new info from the file (that means 2 seconds or other value),
after that the values in this class changed depending on the file.

Now lets dive into the class to expaine each member.

*/


class CpuMonitor{
    

    private :
    // These are some methodes that are used in reading the file,
    // so you don't need to call them or understand them
    static vector <unsigned long> Split(string, bool );
    static vector <unsigned long> Split(string, string, bool);
    static void ReadStatFile();

    // Declare the constructor as private so no one can 
    // create an object of that class
    CpuMonitor(){}

    public :

    // Ok, these are the states that exists in the /proc/stat file 
    // For example the line of the cpu :
    // cpu  754896 217 179956 39586407 6199 0 4706 0 0 0
    // each one of these ten numbers represents a stat
    // the first one for the "user" the second one is the "nice" and so on ...
    // in this enum we have each state has the value of its order
    // we will see where we will use it later in this explination
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

    // this is a vector that stores the total cpu time for each state
    // so if we want for example the total idle time we can do : 
    // CpuMonitor::CPU[3] or we can use the enum that we have created
    // like this : CpuMonitor::CPU[CpuMonitor::States::idle] 
    // so we don't have to remember the index of each state 
    static vector<unsigned long> CPU;
    
    // this is a vector of vectors, it stores the states of each core
    // the count starts by 0 so CORES[0] is the first core,
    // for example we want the idle time of the seconde core : 
    // CpuMonitor::Cores[1][3] or CpuMonitor::CORES[1][CpuMonitor::States::idle]
    // make sure you don't pass the CORES_NUMBER
    static vector<vector<unsigned long>> CORES;

    // The number of cores
    static unsigned int CORES_NUMBER;

    // The number of interrupts
    static unsigned long INTERRUPTS;

    // The number of context switches
    static unsigned long CONTEXT_SWITCHES;

    // The boot time
    static unsigned long BOOT_TIME;

    // The number of processes
    static unsigned long PROCESSES;

    // The number of processes running
    static unsigned long PROCESSES_RUNNING;

    // The number of processes blocked
    static unsigned long PROCESSES_BLOCKED;

    // This is a vector that contains the values of Software interrupts
    // that means (HI, TIMER, NET_TX, NET_RX, BLOCK, IRQ_POLL, TASKLET, SCHED, HRTIMER, RCU)
    // don't use the states enum because its not the case
    static vector<unsigned long> SOFT_IRQS;

    // This methode calls the read file methode 
    // so you can call it whenever you want new values
    static void RefreshCpuMonitor();
    

};

#endif