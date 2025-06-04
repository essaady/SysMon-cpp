#include "../include/SysMon.h"
#include "../include/CpuMonitor.h"
#include "../include/MemoryMonitor.h"
#include "../include/ProcessMonitor.h"

int SysMon::run() {
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    
    while (true) {
        if (!update()) {
            return -1;
        }
        
        // Update all monitors
        cpuMonitor.update();
        memoryMonitor.update();
        processMonitor.update();
        
        // Display information
        std::cout << getInfo("all") << std::endl;
        
        // Wait for next update
        std::this_thread::sleep_for(std::chrono::seconds(updateInterval));
    }
    
    return 0;
}

std::string SysMon::exportAsText() {
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    
    // Update monitors
    cpuMonitor.update();
    memoryMonitor.update();
    processMonitor.update();
    
    std::ostringstream oss;
    oss << "=== SysMon-cpp Export (Text Format) ===\n";
    oss << "Timestamp: " << getTime() << "\n\n";
    
    // CPU Information
    oss << "CPU Information:\n";
    oss << cpuMonitor.getCpuInfo() << "\n";
    
    // Memory Information
    oss << "Memory Information:\n";
    oss << "Total Memory: " << memoryMonitor.getTotalMemory() / 1024 << " MB\n";
    oss << "Used Memory: " << memoryMonitor.getUsedMemory() / 1024 << " MB\n";
    oss << "Free Memory: " << memoryMonitor.getFreeMemory() / 1024 << " MB\n";
    oss << "Memory Usage: " << std::fixed << std::setprecision(2) 
        << memoryMonitor.getMemoryUsagePercentage() << "%\n";
    oss << "Total Swap: " << memoryMonitor.getTotalSwap() / 1024 << " MB\n";
    oss << "Used Swap: " << memoryMonitor.getUsedSwap() / 1024 << " MB\n";
    oss << "Swap Usage: " << std::fixed << std::setprecision(2) 
        << memoryMonitor.getSwapUsagePercentage() << "%\n\n";
    
    // Process Information
    oss << "Process Information:\n";
    oss << processMonitor.getProcessInfo() << "\n";
    
    return oss.str();
}

std::string SysMon::exportAsCSV() {
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    
    // Update monitors
    cpuMonitor.update();
    memoryMonitor.update();
    processMonitor.update();
    
    std::ostringstream oss;
    
    // CSV Header
    oss << "Timestamp,CPU_Usage,CPU_Frequency,Memory_Total_MB,Memory_Used_MB,Memory_Usage_Percent,";
    oss << "Swap_Total_MB,Swap_Used_MB,Swap_Usage_Percent,Process_Count\n";
    
    // CSV Data
    oss << getTime() << ",";
    oss << cpuMonitor.getCpuUsage() << ",";
    oss << cpuMonitor.getCpuFreq() << ",";
    oss << memoryMonitor.getTotalMemory() / 1024 << ",";
    oss << memoryMonitor.getUsedMemory() / 1024 << ",";
    oss << memoryMonitor.getMemoryUsagePercentage() << ",";
    oss << memoryMonitor.getTotalSwap() / 1024 << ",";
    oss << memoryMonitor.getUsedSwap() / 1024 << ",";
    oss << memoryMonitor.getSwapUsagePercentage() << ",";
    oss << processMonitor.getProcessCount() << "\n";
    
    // Process data in CSV format
    oss << "\nProcess Data:\n";
    oss << "PID,User,CPU_Percent,Memory_MB,Time,Command\n";
    oss << processMonitor.getProcessRaw();
    
    return oss.str();
}

bool SysMon::update() {
    // This method can be used for any global updates needed
    // Currently, individual monitors handle their own updates
    return true;
}

std::string SysMon::getTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&time_t);
    
    // Remove newline character
    if (!timeStr.empty() && timeStr.back() == '\n') {
        timeStr.pop_back();
    }
    
    return timeStr;
}

std::string SysMon::getInfo(std::string type) {
    std::ostringstream oss;
    
    if (type == "all" || type == "cpu") {
        CpuMonitor cpuMonitor;
        cpuMonitor.update();
        oss << "=== CPU Information ===\n";
        oss << cpuMonitor.getCpuInfo() << "\n";
    }
    
    if (type == "all" || type == "memory") {
        MemoryMonitor memoryMonitor;
        memoryMonitor.update();
        oss << "=== Memory Information ===\n";
        oss << "Total Memory: " << memoryMonitor.getTotalMemory() / 1024 << " MB\n";
        oss << "Used Memory: " << memoryMonitor.getUsedMemory() / 1024 << " MB\n";
        oss << "Free Memory: " << memoryMonitor.getFreeMemory() / 1024 << " MB\n";
        oss << "Memory Usage: " << std::fixed << std::setprecision(2) 
            << memoryMonitor.getMemoryUsagePercentage() << "%\n";
        oss << "Total Swap: " << memoryMonitor.getTotalSwap() / 1024 << " MB\n";
        oss << "Used Swap: " << memoryMonitor.getUsedSwap() / 1024 << " MB\n";
        oss << "Swap Usage: " << std::fixed << std::setprecision(2) 
            << memoryMonitor.getSwapUsagePercentage() << "%\n\n";
    }
    
    if (type == "all" || type == "processes") {
        ProcessMonitor processMonitor;
        processMonitor.update();
        oss << "=== Process Information ===\n";
        oss << processMonitor.getProcessInfo() << "\n";
    }
    
    return oss.str();
}

void SysMon::log() {
    std::string logData;
    
    if (fullLog) {
        logData = exportAsText();
    } else {
        logData = getTime() + " - System monitored\n";
    }
    
    std::ofstream logFile("sysmon.log", std::ios::app);
    if (logFile.is_open()) {
        logFile << logData << std::endl;
        logFile.close();
    }
}