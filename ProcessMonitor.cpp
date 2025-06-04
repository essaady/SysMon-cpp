activeProcesses ProcessMonitor::getProcess(int index) {
    if (index < 0 || index >= processes.size()) {
        throw std::out_of_range("Index hors limites.");
    }
    return processes[index];
}
std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream info;
    info << "PID\tUser\tCPU%\tMEM%\tPath\t\tTime\n";
    info << "--------------------------------------------------------------\n";
    for (const auto& proc : processes) {
        info << proc.pid << "\t"
             << proc.user << "\t"
             << proc.cpuUsage << "\t"
             << proc.memoryUsage << "\t"
             << proc.pathName << "\t"
             << proc.time << "\n";
    }
    return info.str();
}
