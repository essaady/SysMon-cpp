#include "SysMon.h"
#include <iostream>
#include <chrono>
#include <csignal>
#include <atomic>
#include <thread>
#include <ctime>
#include <sstream>

static std::atomic<bool> keepRunning(true);

void signalHandler(int signum) {
    std::cout << "\n[SysMon] Arrêt demandé (signal " << signum << ").\n";
    keepRunning = false;
}

SysMon::SysMon(int interval, bool log)
    : updateInterval(interval), fullLog(log) {}

SysMon::~SysMon() {}

int SysMon::run() {
    std::signal(SIGINT, signalHandler);
    std::cout << "[SysMon] Surveillance démarrée (intervalle: " << updateInterval << "s)\n";

    while (keepRunning) {
        if (update()) {
            log();
        }
        std::this_thread::sleep_for(std::chrono::seconds(updateInterval));
    }

    std::cout << "[SysMon] Surveillance terminée.\n";
    return 0;
}

bool SysMon::update() {

    std::cout << "[SysMon] Mise à jour des données système...\n";
    return true;
}

std::string SysMon::getTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&t);
}

std::string SysMon::getInfo(const std::string& type) {
    return "[SysMon] Infos : " + type;
}

std::string SysMon::exportAsText() {
    std::cout << "[SysMon] Exportation des données en format texte...\n";
    return "export_text.txt";
}

std::string SysMon::exportAsCSV() {
    std::cout << "[SysMon] Exportation des données au format CSV...\n";
    return "export_data.csv";
}

void SysMon::log() {
    if (fullLog) {
        std::cout << "[SysMon] LOG [" << getTime() << "] : Données enregistrées.\n";
    }
}
