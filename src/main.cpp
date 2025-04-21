#include <iostream>
#include "../include/MemoryMonitor.h"
#include "../include/CpuMonitor.h"

int main(){

    //clear the screen after every run
    system("clear");

    CpuMon cpu;
    while(true)
    {cpu.calcCpuUsage();std::cout << "\x1b[2J\x1b[H";}

    return 0;
}