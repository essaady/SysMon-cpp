#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <iostream>
using namespace std;


bool CpuMonitor::update() {
    // Read CPU 
    ifstream statFyl("/proc/stat");
    if (!statFile.is_open()) return false;

    string line;
    getline(statFyl, line); // Read global CPU 
    istringstream iss(line);

    string label;
    unsigned long user, nice, system, idle
    iss >> label >> user >> nice >> system >> idle ;

    unsigned long total=user + nice + system + idle;

    ifstream cpuFyl("/proc/cpuinfo");
    if (!cpuFyl.is_open()) return false;

    string line;
    getline (cpuFyl,line);
    istringstream iss1(line);
    string cpu_labe1;
    unsigned long user1, nice1, system1, idle1
    iss1 >> label1 >> user1 >> nice1 >> system1 >> idle1 ;
    unsigned long total1=user1 + nice1 + system1 + idle1;

    unsigned long total_diff = total1 - total;
    unsigned long idle_diff  = idle1 - idle;
    if (total_diff > 0) {
        CPU.usageCPU = ((float)(total_diff - idle_diff) / total_diff) * 100.0f; 
        
    } else {
        CPU.usageCPU = 0.0f; //initialise usageCPU 
    }

    return true;
}


float CpuMonitor::getCpuUsage()  {
    return cpu.usageCPU;
}

float CpuMonitor::getCpuFreq()  {
    float freq = 0.0f;
    string label;

    while (cpuinfo >> label) {
    if (label == "cpu") {
    string mhz, colon;
    cpuinfo >> mhz >> colon >> freq;
    break;
    }
    }

    return freq;
}

    

string CpuMonitor::getCpuInfo()  {
   ostringstream oss ;
   oss  << "Cpu :" << Cpu.CpuUsage << "% freq :" << getCpuFreq () << "MHZ";

   return oss.str();



}


