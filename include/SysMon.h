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
#include <fstream>
#include <iostream>

class SysMonitor : public CpuMonitor, public MemoryMonitor, public ProcessMonitor {
private:
    int updateInterval; 
    bool fullLog; 
    std::string exportPath;


    std::string getTime() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    std::string getTimeForFilename() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S");
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

    void setExportPath(const std::string& path) {
        exportPath = path;
        if (!exportPath.empty() && exportPath.back() != '/' && exportPath.back() != '\\') {
            exportPath += "/";
        }
    }

    std::string getExportPath() {
        return exportPath;
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
            return getMemoryInfo();
        }
        else if (type == "process") {
            return getALLProcessInfo();
        }
        return "Unknown info type";
    }

    std::string generateTextReport() {
        std::stringstream ss;
        ss << "=== System Monitor Report ===\n";
        ss << "Time: " << getTime() << "\n\n";
        ss << "--- CPU Information ---\n" << getInfo("cpu") << "\n";
        ss << "--- Memory Information ---\n" << getInfo("memory") << "\n";
        ss << "--- Process Information ---\n" << getInfo("process") << "\n";
        return ss.str();
    }

    std::string generateCSVReport() {
    std::stringstream ss;
    
    
    std::string cpuInfo = getInfo("cpu");
    std::string memInfo = getInfo("memory");
    std::string procInfo = getInfo("process");
    

    ss << "Timestamp,"
       << "CPU Usage (%),"
       << "CPU Frequency (MHz),"
       << "CPU Max Frequency (MHz),"
       << "Number of CPUs,"
       << "Memory Usage (%),"
       << "Memory Used (Bytes),"
       << "Memory Free (Bytes),"
       << "Memory Total (Bytes),"
       << "Memory Used (GB),"
       << "Memory Total (GB),"
       << "Swap Usage (%),"
       << "Swap Used (Bytes),"
       << "Swap Free (Bytes),"
       << "Swap Total (Bytes),"
       << "Swap Used (GB),"
       << "Swap Total (GB),"
       << "Number of Processes,"
       << "CPU Info (Full),"
       << "Memory Info (Full),"
       << "Process Info (Full)\n";
    

    double memUsedGB = getUsedMemory() / (1024.0 * 1024.0 * 1024.0);
    double memTotalGB = getTotalMemory() / (1024.0 * 1024.0 * 1024.0);
    double swapUsedGB = getUsedSwap() / (1024.0 * 1024.0 * 1024.0);
    double swapTotalGB = getTotalSwap() / (1024.0 * 1024.0 * 1024.0);
    

    ss << getTime() << ","
       << getCpuUsage() << ","
       << getCpuFreq() << ","
       << getCpuFreqMax() << "," 
       << getCpuNbr()<< "," 
       << getMemoryUsagePercentage() << ","
       << getUsedMemory() << ","
       << getFreeMemory() << ","
       << getTotalMemory() << ","
       << memUsedGB << ","
       << memTotalGB << ","
       << getSwapUsagePercentage() << ","
       << getUsedSwap() << ","
       << getFreeSwap() << ","
       << getTotalSwap() << ","
       << swapUsedGB << ","
       << swapTotalGB << ","
       << getProcessNbr() << ","
       << cpuInfo << ","
       << memInfo << ","
       << procInfo << "\n";
    
    return ss.str();
}

    bool exportAsText() {
        if (!update()) {
            std::cerr << "Error: Failed to update system information before export" << std::endl;
            return false;
        }

        std::string filename = exportPath + "sysmon_report_" + getTimeForFilename() + ".txt";
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Cannot create file " << filename << std::endl;
            return false;
        }

        file << generateTextReport();
        file.close();

        std::cout << "Text report exported to: " << filename << std::endl;
        return true;
    }

    bool exportAsCSV() {
        if (!update()) {
            std::cerr << "Error: Failed to update system information before export" << std::endl;
            return false;
        }

        std::string filename = exportPath + "sysmon_data_" + getTimeForFilename() + ".csv";
        std::ofstream file(filename);
        
        if (!file.is_open()) {
            std::cerr << "Error: Cannot create file " << filename << std::endl;
            return false;
        }

        file << generateCSVReport();
        file.close();

        std::cout << "CSV data exported to: " << filename << std::endl;
        return true;
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