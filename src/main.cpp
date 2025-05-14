#include <iostream>
#include <thread>
#include <chrono>
#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
using namespace std;

int main() {
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;

    while (true) {
        // Mettre à jour les statistiques
        cpuMonitor.update();
        memoryMonitor.update();
        processMonitor.update();

        // Afficher les résultats
        cout << "\033[2J\033[1;1H"; // Clear console (ANSI escape code)
        cout << "SysMon - System Monitor\n";
        cout << "---------------------------------\n";

        // Afficher l'utilisation CPU
        cout << "CPU Usage: " << cpuMonitor.getCpuUsage() << "%" << endl;

        // Afficher l'utilisation mémoire
        cout << "Memory Usage: " << memoryMonitor.getMemoryUsagePercent() << "%"
                  << " (" << memoryMonitor.getUsedMemoryMB() << " MB used / "
                  << memoryMonitor.getTotalMemoryMB() << " MB total)\n";

        // Afficher les processus
        cout << "\nProcesses:\n";
        cout << "---------------------------------\n";
        for (const auto& proc : processMonitor.getProcesses()) {
            cout << "PID: " << proc.pid << " | Name: " << proc.name << " | CPU: " << proc.cpuUsage << "% | MEM: " << proc.memoryUsageMB << " MB\n";
        }

        // Attendre 1 seconde avant la prochaine mise à jour
        this_thread::sleep_for(chrono::seconds(1));
    }

    return 0;
}
