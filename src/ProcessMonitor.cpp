#include <iostream>
#include <vector>
#include <string>

class ProcessMonitor {
private:
    int* PID;
    int nbrProcess;

    struct AP {
        std::string user;
        float cpu;
        float memory;
        std::string time;
        std::string pathName;
    };

    std::vector<AP> processes;

public:
    ProcessMonitor() : PID(nullptr), nbrProcess(0) {}

    bool update() {
        // Ici on simule une mise à jour des processus
        processes.clear();
        nbrProcess = 2;

        // Processus simulé 1
        AP p1 = {"user1", 12.5f, 100.3f, "00:01:23", "/usr/bin/python3"};
        processes.push_back(p1);

        // Processus simulé 2
        AP p2 = {"user2", 30.2f, 250.8f, "00:03:45", "/usr/bin/firefox"};
        processes.push_back(p2);

        return true;
    }

    AP getProcess(int index) {
        if (index >= 0 && index < processes.size()) {
            return processes[index];
        }
        return AP();  // Retourne un processus vide
    }



    
    std::string getProcessInfo() {
        std::string info;
        for (const auto& p : processes) {
            info += "User: " + p.user +
                    ", CPU: " + std::to_string(p.cpu) +
                    "%, Mem: " + std::to_string(p.memory) +
                    "MB, Time: " + p.time +
                    ", Path: " + p.pathName + "\n";
        }
        return info;
    }
};
