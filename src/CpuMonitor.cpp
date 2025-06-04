#include "ProcessMonitor.h"
#include <sstream>
#include <stdexcept>

activeProcesses ProcessMonitor::getProcess(int index) {
    if (index < 0 || index >= static_cast<int>(processes.size())) {
        throw std::out_of_range("Index de processus invalide");
    }
    return processes[index];
}

std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream oss;

    oss << "PID\tNom\tCPU (%)\tMémoire (MB)\tUtilisateur\tPath\t\t\tTemps\n\n";
    oss << "-------------------------------------------------------------------------------\n";
    for (const auto& proc : processes) {
        oss << proc.pid << "\t"
            << proc.name << "\t"
            << proc.cpu << "\t\t"
            << proc.memory << "\t\t"
            << proc.user << "\t"
            << proc.pathName << "\t"
            << proc.time << "\n";
    }

    return oss.str();
}
