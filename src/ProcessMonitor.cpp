#include "ProcessMonitor.h"
#include <sstream>

ProcessMonitor::ProcessMonitor() {
    update();
}

bool ProcessMonitor::update() {
    activeProcesses.clear();

    AP p1 = {1234, "root", 3.5f, 150.0f, "/usr/bin/ssh", {"08:00", "00:45"}};
    AP p2 = {4321, "user", 1.8f, 90.2f, "/usr/bin/firefox", {"08:30", "00:15"}};

    activeProcesses.push_back(p1);
    activeProcesses.push_back(p2);

    return true;
}

AP ProcessMonitor::getProcess(int index) const {
    if (index < 0 || index >= static_cast<int>(activeProcesses.size())) {
        throw std::out_of_range("Index hors limites");
    }
    return activeProcesses[index];
}

std::string ProcessMonitor::getProcessInfo() const {
    std::ostringstream oss;
    oss << "PID\tUser\tCPU%\tMemoryMB\tPath\t\tStart\tRunTime\n";
    oss << "-----------------------------------------------------------------------\n";

    for (const auto& proc : activeProcesses) {
        oss << proc.pid << "\t"
            << proc.user << "\t"
            << proc.cpu << "\t"
            << proc.memory << "\t\t"
            << proc.pathName << "\t"
            << proc.time.startTime << "\t"
            << proc.time.runTime << "\n";
    }

    return oss.str();
}
