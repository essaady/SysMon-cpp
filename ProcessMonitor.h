struct activeProcesses {
    int pid;
    std::string user;
    float cpuUsage;
    float memoryUsage;
    std::string pathName;
    std::string time;
};
class ProcessMonitor {
private:
    std::vector<activeProcesses> processes;

public:
    void update(); // Remplit `processes` avec les données du système
    activeProcesses getProcess(int index);
    std::string getProcessInfo();
};
