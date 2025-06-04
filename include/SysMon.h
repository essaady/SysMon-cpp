#ifndef SYSMON_H
#define SYSMON_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <iomanip>

class SysMon {
private:
    int updateInterval;
    bool fullLog;

public:
    SysMon(int interval = 1) : updateInterval(interval), fullLog(false) {}
    
    // Core functions
    int run();
    std::string exportAsText();
    std::string exportAsCSV();
    bool update();
    std::string getTime();
    std::string getInfo(std::string type);
    void log();
    
    // Setters
    void setUpdateInterval(int interval) { updateInterval = interval; }
    void setFullLog(bool enable) { fullLog = enable; }
    
    // Getters
    int getUpdateInterval() const { return updateInterval; }
    bool isFullLogEnabled() const { return fullLog; }
};

#endif 