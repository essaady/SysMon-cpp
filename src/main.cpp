#include "../include/SysMon.h"
#include "../include/CpuMonitor.h"
#include "../include/MemoryMonitor.h"
#include "../include/ProcessMonitor.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <cstring>

// Global flag for graceful shutdown
volatile sig_atomic_t running = 1;

void signalHandler(int signal) {
    running = 0;
}

void printUsage() {
    std::cout << "SysMon-cpp - System Monitor\n";
    std::cout << "Usage: ./SysMon-cpp [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --help                Show this help message\n";
    std::cout << "  --export-text         Export data to text file\n";
    std::cout << "  --export-csv          Export data to CSV file\n";
    std::cout << "  --update-interval N   Set update interval in seconds (default: 1)\n";
    std::cout << "  --once                Run once and exit\n";
    std::cout << "\nControls during execution:\n";
    std::cout << "  Ctrl+C                Exit the program\n";
}

void clearScreen() {
    std::cout << "\x1B[2J\x1B[H"; // ANSI escape codes to clear screen
}

void displayHeader() {
    std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                               SysMon-cpp v1.0                                ║\n";
    std::cout << "║                          System Monitor - Press Ctrl+C to exit              ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n\n";
}

int main(int argc, char* argv[]) {
    // Parse command line arguments
    int updateInterval = 1;
    bool exportText = false;
    bool exportCsv = false;
    bool runOnce = false;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0) {
            printUsage();
            return 0;
        } else if (strcmp(argv[i], "--export-text") == 0) {
            exportText = true;
        } else if (strcmp(argv[i], "--export-csv") == 0) {
            exportCsv = true;
        } else if (strcmp(argv[i], "--update-interval") == 0) {
            if (i + 1 < argc) {
                updateInterval = std::atoi(argv[++i]);
                if (updateInterval <= 0) {
                    std::cerr << "Invalid update interval. Using default value of 1 second.\n";
                    updateInterval = 1;
                }
            }
        } else if (strcmp(argv[i], "--once") == 0) {
            runOnce = true;
        } else {
            std::cerr << "Unknown option: " << argv[i] << "\n";
            printUsage();
            return 1;
        }
    }
    
    // Set up signal handler for graceful shutdown
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    // Initialize monitors
    SysMon sysMon(updateInterval);
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    
    std::cout << "Starting SysMon-cpp...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Main monitoring loop
    while (running) {
        // Update all monitors
        cpuMonitor.update();
        memoryMonitor.update();
        processMonitor.update();
        
        // Clear screen and display header
        clearScreen();
        displayHeader();
        
        // Display current time
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "Last Update: " << std::ctime(&time_t);
        
        // Display CPU information
        std::cout << "┌─ CPU Information ─────────────────────────────────────────────────────────┐\n";
        std::cout << "│ Overall CPU Usage: " << std::fixed << std::setprecision(2) 
                  << cpuMonitor.getCpuUsage() << "%";
        std::cout << std::string(46 - std::to_string((int)cpuMonitor.getCpuUsage()).length(), ' ') << "│\n";
        std::cout << "│ CPU Frequency: " << std::fixed << std::setprecision(1) 
                  << cpuMonitor.getCpuFreq() << " MHz";
        std::cout << std::string(51 - std::to_string((int)cpuMonitor.getCpuFreq()).length(), ' ') << "│\n";
        std::cout << "│ CPU Cores: " << cpuMonitor.getCpuCount();
        std::cout << std::string(63 - std::to_string(cpuMonitor.getCpuCount()).length(), ' ') << "│\n";
        
        // Display per-core usage
        for (int i = 0; i < cpuMonitor.getCpuCount() && i < 8; i++) {
            float coreUsage = cpuMonitor.getCpuUsagePerCore(i);
            std::cout << "│ Core " << i << ": " << std::fixed << std::setprecision(1) << coreUsage << "%";
            
            // Simple bar visualization
            int barLength = 30;
            int filled = (int)(coreUsage * barLength / 100);
            std::cout << " [";
            for (int j = 0; j < barLength; j++) {
                if (j < filled) std::cout << "█";
                else std::cout << " ";
            }
            std::cout << "]";
            
            std::cout << std::string(30 - std::to_string(i).length() - std::to_string((int)coreUsage).length(), ' ') << "│\n";
        }
        std::cout << "└───────────────────────────────────────────────────────────────────────────┘\n\n";
        
        // Display Memory information
        std::cout << "┌─ Memory Information ──────────────────────────────────────────────────────┐\n";
        double memUsagePercent = memoryMonitor.getMemoryUsagePercentage();
        unsigned long totalMem = memoryMonitor.getTotalMemory() / 1024; // Convert to MB
        unsigned long usedMem = memoryMonitor.getUsedMemory() / 1024;   // Convert to MB
        unsigned long freeMem = memoryMonitor.getFreeMemory() / 1024;   // Convert to MB
        
        std::cout << "│ Memory Usage: " << std::fixed << std::setprecision(1) << memUsagePercent << "% ";
        std::cout << "(" << usedMem << "MB / " << totalMem << "MB)";
        std::cout << std::string(35 - std::to_string(usedMem).length() - std::to_string(totalMem).length(), ' ') << "│\n";
        
        // Memory bar visualization
        int memBarLength = 50;
        int memFilled = (int)(memUsagePercent * memBarLength / 100);
        std::cout << "│ [";
        for (int j = 0; j < memBarLength; j++) {
            if (j < memFilled) std::cout << "█";
            else std::cout << " ";
        }
        std::cout << "]" << std::string(22, ' ') << "│\n";
        
        // Swap information
        double swapUsagePercent = memoryMonitor.getSwapUsagePercentage();
        unsigned long totalSwap = memoryMonitor.getTotalSwap() / 1024; // Convert to MB
        unsigned long usedSwap = memoryMonitor.getUsedSwap() / 1024;   // Convert to MB
        
        if (totalSwap > 0) {
            std::cout << "│ Swap Usage: " << std::fixed << std::setprecision(1) << swapUsagePercent << "% ";
            std::cout << "(" << usedSwap << "MB / " << totalSwap << "MB)";
            std::cout << std::string(36 - std::to_string(usedSwap).length() - std::to_string(totalSwap).length(), ' ') << "│\n";
        } else {
            std::cout << "│ Swap: Not available" << std::string(54, ' ') << "│\n";
        }
        std::cout << "└───────────────────────────────────────────────────────────────────────────┘\n\n";
        
        // Display Process information
        std::cout << "┌─ Top Processes (by CPU Usage) ────────────────────────────────────────────┐\n";
        std::cout << "│ " << std::left << std::setw(8) << "PID" 
                  << std::setw(10) << "USER" 
                  << std::setw(8) << "CPU%" 
                  << std::setw(10) << "MEM(MB)" 
                  << std::setw(10) << "TIME" 
                  << "COMMAND" << std::string(25, ' ') << "│\n";
        std::cout << "│" << std::string(74, '─') << "│\n";
        
        auto topProcs = processMonitor.getTopProcessesByCpu(10);
        for (const auto& proc : topProcs) {
            std::cout << "│ " << std::left << std::setw(8) << proc.first
                      << std::setw(10) << proc.second.user.substr(0, 9)
                      << std::setw(8) << std::fixed << std::setprecision(1) << proc.second.cpu
                      << std::setw(10) << std::fixed << std::setprecision(1) << proc.second.memory
                      << std::setfill('0') << std::setw(2) << proc.second.time.hours << ":"
                      << std::setw(2) << proc.second.time.minutes << ":"
                      << std::setw(2) << proc.second.time.seconds << std::setfill(' ') << " "
                      << proc.second.pathName.substr(0, 20);
            
            // Calculate remaining space for alignment
            int remaining = 74 - 8 - 10 - 8 - 10 - 9 - std::min(20, (int)proc.second.pathName.length());
            std::cout << std::string(std::max(0, remaining), ' ') << "│\n";
        }
        std::cout << "│ Total Processes: " << processMonitor.getProcessCount();
        std::cout << std::string(56 - std::to_string(processMonitor.getProcessCount()).length(), ' ') << "│\n";
        std::cout << "└───────────────────────────────────────────────────────────────────────────┘\n";
        
        // Export data if requested
        if (exportText) {
            std::string textData = sysMon.exportAsText();
            std::ofstream textFile("sysmon_export.txt");
            if (textFile.is_open()) {
                textFile << "=== SysMon-cpp Export ===\n";
                textFile << "Timestamp: " << std::ctime(&time_t) << "\n";
                textFile << cpuMonitor.getCpuInfo() << "\n";
                textFile << "Memory Usage: " << memUsagePercent << "%\n";
                textFile << "Total Memory: " << totalMem << " MB\n";
                textFile << "Used Memory: " << usedMem << " MB\n";
                textFile << "\nTop Processes:\n";
                textFile << processMonitor.getProcessInfo();
                textFile.close();
                std::cout << "\nData exported to sysmon_export.txt\n";
            }
        }
        
        if (exportCsv) {
            std::ofstream csvFile("sysmon_export.csv");
            if (csvFile.is_open()) {
                csvFile << "Timestamp,CPU_Usage,Memory_Usage,Total_Memory_MB,Used_Memory_MB,Process_Count\n";
                csvFile << std::ctime(&time_t) << "," 
                        << cpuMonitor.getCpuUsage() << ","
                        << memUsagePercent << ","
                        << totalMem << ","
                        << usedMem << ","
                        << processMonitor.getProcessCount() << "\n";
                csvFile.close();
                std::cout << "\nData exported to sysmon_export.csv\n";
            }
        }
        
        // Exit if run once mode
        if (runOnce) {
            break;
        }
        
        // Wait for next update
        std::this_thread::sleep_for(std::chrono::seconds(updateInterval));
    }
    
    std::cout << "\nSysMon-cpp stopped.\n";
    return 0;
}