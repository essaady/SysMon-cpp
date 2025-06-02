
#include "../include/CpuMonitor.h"
#include <fstream>
#include <string>
#include <unistd.h> 
#include <thread>    
#include <chrono>    

float CpuMonitor::getCpuUsage() {
    std::ifstream file("/proc/stat");
    std::string cpu;
    long user, nice, system, idle, iowait, irq, softirq, steal;

    file >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    file.close();

    long idleTime = idle + iowait;
    long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

    long prevIdle = idleTime;
    long prevTotal = totalTime;


    std::this_thread::sleep_for(std::chrono::seconds(1));

    file.open("/proc/stat");
    file >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    file.close();

    idleTime = idle + iowait;
    totalTime = user + nice + system + idle + iowait + irq + softirq + steal;

    long deltaIdle = idleTime - prevIdle;
    long deltaTotal = totalTime - prevTotal;

    float usage = 100.0 * (1.0 - ((float)deltaIdle / deltaTotal));
    return usage;
}
