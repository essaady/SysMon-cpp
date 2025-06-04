#include "SysMon.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <csignal>
#include <ctime>

bool keepRunning = true;

void signalHandler(int signum) {
    keepRunning = false;
}

SysMon::SysMon() {
    updateInterval = 2;
}

SysMon::~SysMon() {}

int SysMon::run() {
    std::signal(SIGINT, signalHandler);

    while (keepRunning) {
        if (update()) {
            log();
        }
        std::this_thread::sleep_for(std::chrono::seconds(updateInterval));
    }

    std::cout << "Monitoring stopped by user." << std::endl;
    return 0;
}

bool SysMon::update() {
    return true;
}

void SysMon::log() {
    std::string data = getTime() + " | Info logged";
    fullLog.push_back(data);
}

std::string SysMon::getTime() {
    std::time_t now = std::time(0);
    char* dt = std::ctime(&now);
    return std::string(dt);
}

std::string SysMon::getInfo(std::string type) {
    return "Dummy info for: " + type;
}

std::string SysMon::exportAsText(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) return "Failed to open file.";

    for (const auto& line : fullLog) {
        file << line << "\n";
    }

    file.close();
    return "Exported to " + filepath;
}

std::string SysMon::exportAsCSV(const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) return "Failed to open file.";

    for (const auto& line : fullLog) {
        file << "\"" << line << "\"\n";
    }

    file.close();
    return "Exported CSV to " + filepath;
}
