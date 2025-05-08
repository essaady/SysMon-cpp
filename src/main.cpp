#include "SysMon.h"
#include "ProcessMonitor.h"
#include <iostream>

int main() {
    auto topProcesses = ProcessMonitor::getTopProcesses(5);
    
    std::cout << std::left << std::setw(8) << "PID" 
            << std::setw(30) << "COMMAND" 
            << "CPU %" << std::endl;
    std::cout << std::string(45, '-') << std::endl;

    for (const auto& proc : topProcesses) {
        std::cout << std::left << std::setw(8) << proc.pid
                << std::setw(30) << 
                (proc.command.length() > 28 ? 
                proc.command.substr(0, 25) + "..." : proc.command)
                << std::fixed << std::setprecision(1) << proc.cpu_usage << "%" 
                << std::endl;
    }

    return 0;
}
