#include "../include/ProcessMonitor.h"
#include "../include/SysMon.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <pwd.h>

using namespace std;

// Helper function: Get username from UID
std::string getUserFromUID(uid_t uid) {
    struct passwd *pw = getpwuid(uid);
    return (pw) ? pw->pw_name : "unknown";
}

// Constructeur
ProcessMonitor::ProcessMonitor() {
    nbrProcess = 0;
}

// Destructeur
ProcessMonitor::~ProcessMonitor() {
    // Libération des ressources si nécessaire
}

std::string ProcessMonitor::getProcessRaw() {
    std::cout << "\x1b[42m Name       " << "PID        \x1b[0m\n";
    std::string path = "/proc";
    int processCount = 0;
    for (auto const &entry : std::filesystem::directory_iterator(path)) {
        std::string newPath = entry.path();
        std::string pathFileName = entry.path().filename();
        if (isNumber(pathFileName)) {
            std::string fileContent = SysMon::getInfo(newPath + "/status");
            if (fileContent == " ") {
                continue;
            }
            std::istringstream iss(fileContent);
            std::vector<std::string> info = getVector(iss);
            processCount++;

            std::cout << info[1] << "    " << info[12] << std::endl;
        }
    }

    std::cout << "There are : " << processCount << " processes\n";
    return " ";
}

bool ProcessMonitor::update() {
    processList.clear();
    std::string path = "/proc";

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        std::string dirName = entry.path().filename();
        if (isNumber(dirName)) {
            int pid = std::stoi(dirName);
            std::string statusPath = entry.path() / "status";
            std::string exePath = "";

            try {
                exePath = std::filesystem::read_symlink(entry.path() / "exe").string();
            } catch (...) {
                exePath = "unknown";
            }

            std::string fileContent = SysMon::getInfo(statusPath);
            if (fileContent.empty()) continue;

            std::istringstream iss(fileContent);
            std::vector<std::string> info = getVector(iss);

            activeProcesses proc;
            proc.pid = pid;
            proc.pathName = exePath;
            proc.user = getUserFromUID(std::stoi(info[8])); // Adjust index if needed
            proc.memory = std::stof(info[15]);              // Adjust index if needed
            proc.cpu = 0.0f;                                // Placeholder
            proc.elapsedTime = "N/A";                       // Optional for now

            processList.push_back(proc);
        }
    }

    nbrProcess = processList.size();
    return true;
}

activeProcesses ProcessMonitor::getProcess(int index) {
    if (index >= 0 && index < processList.size()) {
        return processList[index];
    } else {
        return activeProcesses{};  // Empty/default process
    }
}

std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream output;
    output << "PID\tUSER\tMEMORY(kB)\tCPU(%)\tPATH\n";
    for (const auto& proc : processList) {
        output << proc.pid << "\t"
               << proc.user << "\t"
               << proc.memory << "\t"
               << proc.cpu << "\t"
               << proc.pathName << "\n";
    }
    return output.str();
}
