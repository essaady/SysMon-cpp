#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <iostream>
#include <fstream>
#include <string>

class MemoryMonitor {
public:
    MemoryMonitor();
    void updateMemoryUsage();
    int getTotalMemory();
    int getUsedMemory();
    void readProcFile(const std::string& filePath);

private:
    int totalMemory;
    int usedMemory;
    void parseMemInfo();
};

#endif 
