#include "../include/SysMon.h"
#include <unistd.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstring>

void printHelp() {
    std::cout << "SysMon-cpp - System Monitoring Tool\n"
              << "---------------------------------\n"
              << "Usage: ./SysMon-cpp [options]\n\n"
              << "Options:\n"
              << "  --help        : Show this help message\n"
              << "  --update [sec]: Set update interval in seconds (default: 0.5)\n"
              << "  --export      : Export system stats to files\n"
              << "  --process [n] : Show top n processes (default: 10)\n"
              << "  --sort-cpu    : Sort processes by CPU usage (default)\n"
              << "  --sort-memory : Sort processes by memory usage\n"
              << "  --csv         : Export data in CSV format\n"
              << "  --continuous  : Run in continuous mode until Ctrl+C\n"
              << std::endl;
}

void exportToFile(const SysMon& monitor, bool csvFormat) {
    std::string extension = csvFormat ? ".csv" : ".txt";
    std::time_t now = std::time(nullptr);
    std::string timestamp = std::to_string(now);
    
    if (csvFormat) {
        std::ofstream cpuFile("cpu_stats_" + timestamp + extension);
        std::ofstream memFile("mem_stats_" + timestamp + extension);
        std::ofstream procFile("proc_stats_" + timestamp + extension);
        
        // Write CSV headers and data
        // This would need to be implemented in the respective monitor classes
        std::cout << "Exported data in CSV format" << std::endl;
    } else {
        std::ofstream reportFile("system_report_" + timestamp + extension);
        
        reportFile << "SYSTEM REPORT - " << timestamp << "\n\n";
        // Add CPU, Memory, Process information
        reportFile.close();
        std::cout << "Exported data as text report" << std::endl;
    }
}

int main(int argc, char *argv[])
{
    system("clear");
    vector<string> options = {"--help", "--update", "--export", "--process", "--sort-cpu", "--sort-memory", "--csv", "--continuous"};
    
    int option = 0;
    int updateInterval = 5e5; // 0.5 seconds default
    int processLimit = 10;    // Default number of processes to show
    bool exportStats = false;
    bool csvFormat = false;
    bool sortByCpu = true;
    bool continuous = false;
    
    for (int i = 1; i < argc; i++) {
        bool recognized = false;
        for (auto& param : options) {
            if (strcmp(argv[i], param.c_str()) == 0) {
                recognized = true;
                
                if (param == "--help") {
                    printHelp();
                    return 0;
                } else if (param == "--update" && i + 1 < argc) {
                    try {
                        float seconds = std::stof(argv[i+1]);
                        updateInterval = static_cast<int>(seconds * 1e6);
                        i++; // Skip the next argument
                    } catch (...) {
                        std::cerr << "Invalid update interval\n";
                        return -1;
                    }
                } else if (param == "--export") {
                    exportStats = true;
                    option = options::_NLOG;
                } else if (param == "--process" && i + 1 < argc) {
                    try {
                        processLimit = std::stoi(argv[i+1]);
                        i++; // Skip the next argument
                    } catch (...) {
                        std::cerr << "Invalid process count\n";
                        return -1;
                    }
                } else if (param == "--sort-cpu") {
                    sortByCpu = true;
                } else if (param == "--sort-memory") {
                    sortByCpu = false;
                } else if (param == "--csv") {
                    csvFormat = true;
                } else if (param == "--continuous") {
                    continuous = true;
                }
                
                break;
            }
        }
        
        if (!recognized) {
            std::cout << "'" << argv[i] << "' command not recognized\n";
            std::cout << "Use --help for available options\n";
            return -1;
        }
    }
    
    SysMon sysmon(updateInterval);
    
    // Configure process monitor
    ProcessMonitor* procMon = dynamic_cast<ProcessMonitor*>(&sysmon);
    if (procMon) {
        procMon->setMaxProcessesToShow(processLimit);
        procMon->setSortByCpu(sortByCpu);
    }
    
    if (exportStats) {
        exportToFile(sysmon, csvFormat);
        return 0;
    }
    
    if (continuous) {
        // Run indefinitely until user terminates
        return sysmon.run(-1);
    } else {
        // Run with default number of updates
        return sysmon.run();
    }
}