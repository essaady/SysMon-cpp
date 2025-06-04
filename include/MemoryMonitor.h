#ifndef MEMORYMONITOR_H
#define MEMORYMONITOR_H

#include <string>

class MemoryMonitor {
public:
    MemoryMonitor() = default;
    void update();
    float getMemoryUsage() const;
    std::string getMemoryInfo() const;

private:
    float memoryUsage = 0.0f;
};

#endif
