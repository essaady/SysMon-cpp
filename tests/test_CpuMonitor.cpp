#include "../include/CpuMonitor.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    CpuMonitor cpu;

    std::cout << ">>> Test du module CpuMonitor <<<" << std::endl;

    if (!cpu.update()) {
        std::cerr << "Erreur : Impossible de lire /proc/stat" << std::endl;
        return 1;
    }

    std::cout << "Lecture initiale reussie. Attente de 1 seconde..." << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    if (!cpu.update()) {
        std::cerr << "Erreur : Mise a jour echouee." << std::endl;
        return 1;
    }

    std::cout << "Utilisation CPU : " << cpu.getCpuUsage() << " %" << std::endl;
    std::cout << "Frequence CPU : " << cpu.getCpuFreq() << " MHz" << std::endl;

    std::cout << "Infos brutes : " << cpu.getCpuInfo() << std::endl;

    return 0;
}
