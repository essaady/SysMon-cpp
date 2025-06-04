#include "SysMon.h"
#include "CPU.h"
#include "Memory.h"
#include "Process.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <thread>

SysMon::SysMon(int interval) : updateInterval(interval) {}

bool SysMon::update() {
    CPU cpu;
    Memory memory;
    Process proc;

    float cpuUsage = cpu.getUsage();
    float memUsage = memory.getUsage();
    proc.update();

    std::ostringstream oss;
    oss << "[" << getTime() << "] "
        << "CPU: " << cpuUsage << "%, "
        << "Memory: " << memUsage << "%\n"
        << proc.getProcessInfo() << "\n";

    fullLog += oss.str();
    return true;
}

std::string SysMon::getTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char buffer[26];
    ctime_r(&now_time, buffer);
    buffer[24] = '\0'; // remove newline
    return std::string(buffer);
}

int SysMon::run() {
    while (true) {
        system("clear");
        std::cout << "=== SysMon-cpp (Advanced) ===" << std::endl;
        update();
        std::cout << getInfo("text") << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(updateInterval));
    }
    return 0;
}

std::string SysMon::getInfo(const std::string& format) {
    if (format == "csv") return exportAsCSV();
    return exportAsText();
}

std::string SysMon::exportAsText() {
    return fullLog;
}

std::string SysMon::exportAsCSV() {
    std::ostringstream oss;
    oss << "Timestamp,CPU,Memory\n";

    std::istringstream iss(fullLog);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find("CPU:") != std::string::npos) {
            std::string time = line.substr(1, 24);
            size_t cpuPos = line.find("CPU:");
            size_t memPos = line.find("Memory:");
            std::string cpu = line.substr(cpuPos + 5, line.find("%") - cpuPos - 5);
            std::string mem = line.substr(memPos + 8, line.find("%", memPos) - memPos - 8);
            oss << time << "," << cpu << "," << mem << "\n";
        }
    }

    return oss.str();
}

void SysMon::log() {
    std::ofstream file("SysMonLog.txt");
    if (file.is_open()) {
        file << fullLog;
        file.close();
    }
}
