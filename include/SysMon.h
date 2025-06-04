#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"

#include <string>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>
#include <ctime>

class SysMonitor : public CpuMonitor, public MemoryMonitor, public ProcessMonitor {
private:
    int updateInterval; 
    bool fullLog; 

    std::string getTime() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void displayHeader() {
        std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                           SYSMON-CPP - System Monitor                        ║" << std::endl;
        std::cout << "║                              Press Ctrl+C to exit                            ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << std::endl;
        std::cout << std::endl;
    }

public:
    SysMonitor(int interval = 1000, bool enableFullLog = false) 
        : CpuMonitor(), 
          MemoryMonitor(), 
          ProcessMonitor(),
          updateInterval(interval),
          fullLog(enableFullLog) {}

    void setUpdateInterval(int interval) {
        updateInterval = interval;
    }
    int getUpdateInterval() {
        return updateInterval ;
    }

    void setFullLogging(bool enable) {
        fullLog = enable;
    }
    bool getFullLogging() {
        return fullLog;
    }

    bool update() {
        return CpuMonitor::update() && 
               MemoryMonitor::update() && 
               ProcessMonitor::update();
    }

    std::string getInfo(std::string type) {
        if (type == "cpu") {
            return getCpuInfo();
        }
        else if (type == "memory") {
            return "Memory Usage: " + std::to_string(getMemoryUsagePercentage()) + "% "
                    + "||||||" +
                   "Swap Usage: " + std::to_string(getSwapUsagePercentage()) + "%\n";
        }
        else if (type == "process") {
            return "Proc info : " + getProcessInfo() + 
                   "\nProc raw: " + getProcessRaw();
        }
        return "Unknown info type";
    }

    std::string exportAsText() {
        return "";
    }

    std::string exportAsCSV() {
        return "";
    }


    int run() {
        try {
            while (true) {
                clearScreen();
                displayHeader();
                
                if(update()){
                    std::cout << "Time: " << getTime() << std::endl;
                    std::cout << "────────────────────────────────────────────────────────────────────────────────" << std::endl;
                    
                    std::cout << "=== CPU information ===" << std::endl;
                    std::cout << getInfo("cpu") << std::endl;
                    
                    std::cout << "=== Memory information ===" << std::endl;
                    std::cout << getInfo("memory") << std::endl;
                    
                    std::cout << "=== process information ===" << std::endl;
                    std::cout << getInfo("process") << std::endl;
                    
                    std::cout << "────────────────────────────────────────────────────────────────────────────────" << std::endl;
                    std::cout << "Update interval: " << updateInterval << "every 1 sec..." << std::endl;
                
                
                    std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
                } else {
                    std::cout << "error: Failed to update system information" << std::endl;
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }
        return 0;
    }

    ~SysMonitor(){};
};

#endif