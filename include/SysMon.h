#pragma once
#ifndef SYSMON_H
#define SYSMON_H

#include "./CpuMonitor.h"
#include "./MemoryMonitor.h"
#include "./ProcessMonitor.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstring>
#include <vector>
#include <filesystem>

using namespace std;

enum options
{
    _NLOG = 1000
};

std::vector<std::string> getVector(std::istringstream &iss);
bool isNumber(std::string &string);

class SysMon : public CpuMonitor, public MemoryMonitor, public ProcessMonitor {
protected:
    int updateInterval;    // Update interval in microseconds
    bool fullLog;         // Full logging flag
    
public:
    // Constructor and destructor
    SysMon(int interval, bool fullLog=false);
    ~SysMon();

    // Core functionality
    int run(int limit=3);
    bool update();

    // Export functionality
    string exportAsText();
    string exportAsCSV();

    // Utility functions
    string getTime();
    static string getInfo(string filePath);
    static void log(ostream& out);

private:
    // Helper functions
    string formatOutput() const;
    void initializeMonitors();

};
#endif