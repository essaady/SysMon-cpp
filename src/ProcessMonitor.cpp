std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream oss;
    oss << "User\tMemory\tPath\tTime\n";
    for (const auto& proc : processList) {
        oss << proc.user << "\t"
            << proc.memory << "\t"
            << proc.pathName << "\t"
            << std::put_time(&proc.time, "%H:%M:%S") << "\n";
    }
    return oss.str();
}
activeProcesses ProcessMonitor::getProcess(int index) {
    if (index >= 0 && index < processList.size()) {
        return processList[index];
    } else {
        throw std::out_of_range("Index invalide");
    }
}