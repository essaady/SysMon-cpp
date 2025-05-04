#include "../include/SysMon.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <iomanip>

SysMon::SysMon(int updateInterval) 
    : updateInterval(updateInterval), running(false) {
    // Initialisation
    update();
}

SysMon::~SysMon() {
    stop();
}

void SysMon::start() {
    if (!running) {
        running = true;
        std::thread monitorThread(&SysMon::monitorLoop, this);
        monitorThread.detach();
    }
}

void SysMon::stop() {
    running = false;
}

bool SysMon::update() {
    bool cpuSuccess = cpuMonitor.update();
    bool memSuccess = memoryMonitor.update();
    bool procSuccess = processMonitor.update();
    
    lastUpdateTime = std::chrono::steady_clock::now();
    
    return cpuSuccess && memSuccess && procSuccess;
}

void SysMon::display(int showProcessCount) const {
    // Afficher l'heure actuelle
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    std::cout << "SysMon - " << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d %H:%M:%S") << "\n\n";
    
    // Afficher les informations CPU
    std::cout << "CPU Usage: " << std::fixed << std::setprecision(2) << cpuMonitor.getTotalUsage() << "%\n";
    
    auto coreUsage = cpuMonitor.getCoreUsage();
    for (const auto& pair : coreUsage) {
        std::cout << "  " << pair.first << ": " << std::fixed << std::setprecision(2) << pair.second << "%\n";
    }
    std::cout << "\n";
    
    // Afficher les informations mémoire
    const double KB_TO_MB = 1024.0;
    std::cout << "Memory: " << std::fixed << std::setprecision(2) 
              << memoryMonitor.getUsedMemory() / KB_TO_MB << " MB / " 
              << memoryMonitor.getTotalMemory() / KB_TO_MB << " MB ("
              << std::fixed << std::setprecision(2) << memoryMonitor.getMemoryUsagePercentage() << "%)\n";
    
    std::cout << "Swap: " << std::fixed << std::setprecision(2) 
              << memoryMonitor.getUsedSwap() / KB_TO_MB << " MB / " 
              << memoryMonitor.getTotalSwap() / KB_TO_MB << " MB ("
              << std::fixed << std::setprecision(2) << memoryMonitor.getSwapUsagePercentage() << "%)\n\n";
    
    // Afficher les informations processus
    std::cout << "Top " << showProcessCount << " processes by CPU usage:\n";
    std::cout << std::setw(7) << "PID" << " | " 
              << std::setw(15) << "USER" << " | " 
              << std::setw(8) << "CPU%" << " | " 
              << std::setw(8) << "MEM%" << " | " 
              << std::setw(8) << "RSS(MB)" << " | " 
              << std::setw(5) << "THR" << " | " 
              << "COMMAND\n";
    
    std::cout << std::string(80, '-') << "\n";
    
    auto topProcesses = processMonitor.getTopCPUProcesses(showProcessCount);
    for (const auto& process : topProcesses) {
        double rssMB = process.rss / 1024.0;
        
        std::cout << std::setw(7) << process.pid << " | " 
                  << std::setw(15) << process.user << " | " 
                  << std::setw(7) << std::fixed << std::setprecision(1) << process.cpuUsage << "% | " 
                  << std::setw(7) << std::fixed << std::setprecision(1) << process.memoryUsage << "% | " 
                  << std::setw(7) << std::fixed << std::setprecision(1) << rssMB << " | " 
                  << std::setw(5) << process.threads << " | " 
                  << process.name << "\n";
    }
}

bool SysMon::exportData(const std::string& filename, ExportFormat format, int showProcessCount) const {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }
    
    // Exporter les données selon le format demandé
    if (format == ExportFormat::CSV) {
        outFile << "# SysMon Export - CSV Format\n\n";
        outFile << "# CPU Usage\n";
        outFile << cpuMonitor.exportAsCSV();
        outFile << "\n# Memory Usage\n";
        outFile << memoryMonitor.exportAsCSV();
        outFile << "\n# Process Information\n";
        outFile << processMonitor.exportAsCSV(showProcessCount);
    } else {
        outFile << "SysMon Export - Text Format\n\n";
        outFile << "=== CPU Usage ===\n";
        outFile << cpuMonitor.exportAsText();
        outFile << "\n=== Memory Usage ===\n";
        outFile << memoryMonitor.exportAsText();
        outFile << "\n=== Process Information ===\n";
        outFile << processMonitor.exportAsText(showProcessCount);
    }
    
    outFile.close();
    return true;
}

const CpuMonitor& SysMon::getCpuMonitor() const {
    return cpuMonitor;
}

const MemoryMonitor& SysMon::getMemoryMonitor() const {
    return memoryMonitor;
}

const ProcessMonitor& SysMon::getProcessMonitor() const {
    return processMonitor;
}

void SysMon::setUpdateInterval(int interval) {
    updateInterval = interval;
}

int SysMon::getUpdateInterval() const {
    return updateInterval;
}

void SysMon::monitorLoop() {
    while (running) {
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
    }
}
