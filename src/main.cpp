#include <iostream>
#include <unistd.h>  
#include "ProcessMonitor.h"
using namespace std ;

int main() {
    ProcessMonitor procMon;  

    while (true) {  
        // top 3
        auto topProcesses = procMon.getTopMemoryProcesses(3);

        cout << "Top 3 (RAM):\n";
        for (const auto& proc : topProcesses) {
            cout << "PID " << proc.pid << " | "
                      << proc.name << " | "
                      << proc.memory_usage << " KB\n";
        }

        sleep(2);  // 2 sec
    }
    return 0;
}