#include "CpuMonitor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

CpuMonitor::CpuMonitor() {
    CPU.nbrCPU = 0;
    CPU.usagePerCPU = nullptr; 
    CPU.usageCPU = 0.0;
    CPU.frequency = 0.0;
    CPU.frequencyMax = 0.0;
}

bool CpuMonitor::update() {
    
    
    return true; 
}

float CpuMonitor::getCpuUsage() {
    return CPU.usageCPU;
}

float CpuMonitor::getCpuFreq() {
    return CPU.frequency;
}

string CpuMonitor::getCpuInfo() {
    
    
    return "cpu work";
}

CpuMonitor::~CpuMonitor() {}