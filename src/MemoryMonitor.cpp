#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#pragma comment(lib, "psapi.lib")
#elif __linux__
#include <unistd.h>
#include <sys/sysinfo.h>
#include <dirent.h>     
#endif

class MemoryMonitor {
private:
    struct SystemMemoryInfo {
        size_t totalPhysical;
        size_t availablePhysical;
        size_t usedPhysical;
        size_t totalVirtual;
        size_t availableVirtual;
        size_t usedVirtual;
        double physicalUsagePercent;
        double virtualUsagePercent;
    };

    struct ProcessMemoryInfo {
        std::string processName;
        unsigned long processId;
        size_t workingSetSize;
        size_t virtualMemorySize;
        size_t privateMemorySize;
    };

    std::vector<ProcessMemoryInfo> processes;
    SystemMemoryInfo systemInfo;
    bool monitoring;
    int refreshInterval;

public:
    MemoryMonitor(int interval = 1000) : refreshInterval(interval), monitoring(false) {}

    // Convert bytes to human-readable format
    std::string formatBytes(size_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unitIndex = 0;
        double size = static_cast<double>(bytes);
        
        while (size >= 1024.0 && unitIndex < 4) {
            size /= 1024.0;
            unitIndex++;
        }
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];
        return oss.str();
    }

    // Get system memory information
    bool getSystemMemoryInfo() {
#ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        
        if (!GlobalMemoryStatusEx(&memInfo)) {
            return false;
        }
        
        systemInfo.totalPhysical = memInfo.ullTotalPhys;
        systemInfo.availablePhysical = memInfo.ullAvailPhys;
        systemInfo.usedPhysical = systemInfo.totalPhysical - systemInfo.availablePhysical;
        systemInfo.totalVirtual = memInfo.ullTotalVirtual;
        systemInfo.availableVirtual = memInfo.ullAvailVirtual;
        systemInfo.usedVirtual = systemInfo.totalVirtual - systemInfo.availableVirtual;
        
#elif __linux__
        struct sysinfo si;
        if (sysinfo(&si) != 0) {
            return false;
        }
        
        systemInfo.totalPhysical = si.totalram * si.mem_unit;
        systemInfo.availablePhysical = si.freeram * si.mem_unit;
        systemInfo.usedPhysical = systemInfo.totalPhysical - systemInfo.availablePhysical;
        systemInfo.totalVirtual = si.totalswap * si.mem_unit;
        systemInfo.availableVirtual = si.freeswap * si.mem_unit;
        systemInfo.usedVirtual = systemInfo.totalVirtual - systemInfo.availableVirtual;
#endif
        
        systemInfo.physicalUsagePercent = 
            (static_cast<double>(systemInfo.usedPhysical) / systemInfo.totalPhysical) * 100.0;
        systemInfo.virtualUsagePercent = 
            systemInfo.totalVirtual > 0 ? 
            (static_cast<double>(systemInfo.usedVirtual) / systemInfo.totalVirtual) * 100.0 : 0.0;
        
        return true;
    }

    // Get process memory information
    bool getProcessMemoryInfo() {
        processes.clear();
        
#ifdef _WIN32
        HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hProcessSnap == INVALID_HANDLE_VALUE) {
            return false;
        }
        
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        
        if (!Process32First(hProcessSnap, &pe32)) {
            CloseHandle(hProcessSnap);
            return false;
        }
        
        do {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
                                       FALSE, pe32.th32ProcessID);
            if (hProcess) {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    ProcessMemoryInfo info;
                    info.processName = pe32.szExeFile;
                    info.processId = pe32.th32ProcessID;
                    info.workingSetSize = pmc.WorkingSetSize;
                    info.virtualMemorySize = pmc.PagefileUsage;
                    info.privateMemorySize = pmc.WorkingSetSize;
                    processes.push_back(info);
                }
                CloseHandle(hProcess);
            }
        } while (Process32Next(hProcessSnap, &pe32));
        
        CloseHandle(hProcessSnap);
        
#elif __linux__
        DIR* procDir = opendir("/proc");
        if (!procDir) return false;
        
        struct dirent* entry;
        while ((entry = readdir(procDir)) != nullptr) {
            if (strspn(entry->d_name, "0123456789") == strlen(entry->d_name)) {
                std::string statusPath = "/proc/" + std::string(entry->d_name) + "/status";
                std::string statmPath = "/proc/" + std::string(entry->d_name) + "/statm";
                
                std::ifstream statusFile(statusPath);
                std::ifstream statmFile(statmPath);
                
                if (statusFile.is_open() && statmFile.is_open()) {
                    ProcessMemoryInfo info;
                    info.processId = std::stoul(entry->d_name);
                    
                    std::string line;
                    while (std::getline(statusFile, line)) {
                        if (line.substr(0, 5) == "Name:") {
                            info.processName = line.substr(6);
                            break;
                        }
                    }
                    
                    size_t vmSize, vmRSS;
                    if (statmFile >> vmSize >> vmRSS) {
                        info.virtualMemorySize = vmSize * getpagesize();
                        info.workingSetSize = vmRSS * getpagesize();
                        info.privateMemorySize = info.workingSetSize;
                        processes.push_back(info);
                    }
                }
            }
        }
        closedir(procDir);
#endif
        
        // Sort processes by memory usage (descending)
        std::sort(processes.begin(), processes.end(), 
                 [](const ProcessMemoryInfo& a, const ProcessMemoryInfo& b) {
                     return a.workingSetSize > b.workingSetSize;
                 });
        
        return true;
    }

    // Display system memory information
    void displaySystemInfo() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "           SYSTEM MEMORY INFORMATION" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        std::cout << std::left << std::setw(25) << "Total Physical Memory:" 
                  << formatBytes(systemInfo.totalPhysical) << std::endl;
        std::cout << std::left << std::setw(25) << "Used Physical Memory:" 
                  << formatBytes(systemInfo.usedPhysical) 
                  << " (" << std::fixed << std::setprecision(1) 
                  << systemInfo.physicalUsagePercent << "%)" << std::endl;
        std::cout << std::left << std::setw(25) << "Available Physical:" 
                  << formatBytes(systemInfo.availablePhysical) << std::endl;
        
        if (systemInfo.totalVirtual > 0) {
            std::cout << std::left << std::setw(25) << "Total Virtual Memory:" 
                      << formatBytes(systemInfo.totalVirtual) << std::endl;
            std::cout << std::left << std::setw(25) << "Used Virtual Memory:" 
                      << formatBytes(systemInfo.usedVirtual) 
                      << " (" << std::fixed << std::setprecision(1) 
                      << systemInfo.virtualUsagePercent << "%)" << std::endl;
        }
        
        // Memory usage bar
        std::cout << "\nMemory Usage: [";
        int barWidth = 40;
        int filledWidth = static_cast<int>((systemInfo.physicalUsagePercent / 100.0) * barWidth);
        for (int i = 0; i < barWidth; ++i) {
            if (i < filledWidth) std::cout << "█";
            else std::cout << "░";
        }
        std::cout << "] " << std::fixed << std::setprecision(1) 
                  << systemInfo.physicalUsagePercent << "%" << std::endl;
    }

    // Display top memory-consuming processes
    void displayTopProcesses(int count = 10) {
        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << "           TOP " << count << " MEMORY-CONSUMING PROCESSES" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        
        std::cout << std::left << std::setw(8) << "PID" 
                  << std::setw(25) << "Process Name"
                  << std::setw(15) << "Working Set"
                  << std::setw(15) << "Virtual Mem"
                  << std::setw(15) << "Private Mem" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        int displayCount = std::min(count, static_cast<int>(processes.size()));
        for (int i = 0; i < displayCount; ++i) {
            const auto& proc = processes[i];
            std::cout << std::left << std::setw(8) << proc.processId
                      << std::setw(25) << (proc.processName.length() > 24 ? 
                                          proc.processName.substr(0, 21) + "..." : proc.processName)
                      << std::setw(15) << formatBytes(proc.workingSetSize)
                      << std::setw(15) << formatBytes(proc.virtualMemorySize)
                      << std::setw(15) << formatBytes(proc.privateMemorySize) << std::endl;
        }
    }

    // Find specific process by name
    void findProcess(const std::string& processName) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "         SEARCH RESULTS FOR: " << processName << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        bool found = false;
        for (const auto& proc : processes) {
            if (proc.processName.find(processName) != std::string::npos) {
                if (!found) {
                    std::cout << std::left << std::setw(8) << "PID" 
                              << std::setw(25) << "Process Name"
                              << std::setw(15) << "Working Set" << std::endl;
                    std::cout << std::string(50, '-') << std::endl;
                    found = true;
                }
                std::cout << std::left << std::setw(8) << proc.processId
                          << std::setw(25) << proc.processName
                          << std::setw(15) << formatBytes(proc.workingSetSize) << std::endl;
            }
        }
        
        if (!found) {
            std::cout << "No processes found matching '" << processName << "'" << std::endl;
        }
    }

    // Start continuous monitoring
    void startMonitoring() {
        monitoring = true;
        std::cout << "Starting memory monitoring (Press Ctrl+C to stop)..." << std::endl;
        
        while (monitoring) {
            // Clear screen (cross-platform)
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::cout << "Memory Monitor - " << std::ctime(&time_t);
            
            if (getSystemMemoryInfo() && getProcessMemoryInfo()) {
                displaySystemInfo();
                displayTopProcesses(15);
            } else {
                std::cout << "Error retrieving memory information!" << std::endl;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(refreshInterval));
        }
    }

    // Stop monitoring
    void stopMonitoring() {
        monitoring = false;
    }

    // Get memory statistics
    void getMemoryStats() {
        if (getSystemMemoryInfo() && getProcessMemoryInfo()) {
            displaySystemInfo();
            displayTopProcesses();
        } else {
            std::cout << "Error retrieving memory information!" << std::endl;
        }
    }

    // Set refresh interval
    void setRefreshInterval(int interval) {
        refreshInterval = interval;
    }
};

// Menu system
class MemoryMonitorApp {
private:
    MemoryMonitor monitor;

public:
    void showMenu() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "            MEMORY MONITOR" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. View Current Memory Status" << std::endl;
        std::cout << "2. Start Real-time Monitoring" << std::endl;
        std::cout << "3. Find Process by Name" << std::endl;
        std::cout << "4. Set Refresh Interval" << std::endl;
        std::cout << "5. Export Memory Report" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Enter your choice (1-6): ";
    }

    void exportReport() {
        std::ofstream file("memory_report.txt");
        if (!file.is_open()) {
            std::cout << "Error creating report file!" << std::endl;
            return;
        }

        // Redirect cout to file temporarily
        std::streambuf* orig = std::cout.rdbuf();
        std::cout.rdbuf(file.rdbuf());

        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "Memory Report Generated: " << std::ctime(&time_t) << std::endl;

        monitor.getMemoryStats();

        // Restore cout
        std::cout.rdbuf(orig);
        file.close();

        std::cout << "Memory report exported to 'memory_report.txt'" << std::endl;
    }

    void run() {
        int choice;
        std::string processName;
        int interval;

        while (true) {
            showMenu();
            std::cin >> choice;

            switch (choice) {
                case 1:
                    monitor.getMemoryStats();
                    break;
                case 2:
                    monitor.startMonitoring();
                    break;
                case 3:
                    std::cout << "Enter process name to search: ";
                    std::cin.ignore();
                    std::getline(std::cin, processName);
                    if (monitor.getProcessMemoryInfo()) {
                        monitor.findProcess(processName);
                    }
                    break;
                case 4:
                    std::cout << "Enter refresh interval in milliseconds (default 1000): ";
                    std::cin >> interval;
                    monitor.setRefreshInterval(interval);
                    std::cout << "Refresh interval set to " << interval << "ms" << std::endl;
                    break;
                case 5:
                    exportReport();
                    break;
                case 6:
                    std::cout << "Exiting Memory Monitor..." << std::endl;
                    return;
                default:
                    std::cout << "Invalid choice! Please enter 1-6." << std::endl;
            }

            if (choice != 2 && choice != 6) {
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
        }
    }
};

int main() {
    try {
        MemoryMonitorApp app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
