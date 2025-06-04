#include "../include/CpuMonitor.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

// Constructeur
CpuMonitor::CpuMonitor() {
    // Initialisation des ressources si nécessaire
    CPU.frequencyMax = 0; //To-Do 
    CPU.nbrCPU = 0;  //To-Do
}

// Méthode pour récupérer l'utilisation du CPU (sera implémentée plus tard)
float CpuMonitor::getCpuUsage() {
    // Retourner une valeur factice pour l'instant (par exemple, 0.0)
    //To-Do
    return 0.0;
}

// Destructeur
CpuMonitor::~CpuMonitor() {
    // Libération des ressources si nécessaire

}

bool CpuMonitor::update(){
    CPU.frequency = getCpuFreq();
    return true;
}

float CpuMonitor::getCpuFreq() {
    std::ifstream cpuinfo_file("/proc/cpuinfo");
    std::string line;
    float frequency = 0.0f; // Initialize to 0.0f

    // --- DEBUGGING START ---
    std::cout << "Attempting to open /proc/cpuinfo..." << std::endl;
    // --- DEBUGGING END ---

    if (cpuinfo_file.is_open()) {
        // --- DEBUGGING START ---
        std::cout << "Accessing /proc/cpuinfo opened successfully." << std::endl;
        // --- DEBUGGING END ---

        while (std::getline(cpuinfo_file, line)) {
            // --- DEBUGGING START ---
            // std::cout << "DEBUG: Reading line: " << line << std::endl; // Uncomment for very verbose output
            // --- DEBUGGING END ---

            if (line.rfind("cpu MHz", 0) == 0) { // Check if line starts with "cpu MHz"
                std::istringstream iss(line);
                std::string key_part1, key_part2, colon_delimiter; // "cpu", "MHz", ":"
                iss >> key_part1 >> key_part2 >> colon_delimiter >> frequency; // Read into parts and then frequency

                // --- DEBUGGING START ---
                std::cout << "Found 'cpu MHz' line: '" << line << "'" << std::endl;
                std::cout << "Extracted frequency (before return): " << frequency << std::endl;
                // --- DEBUGGING END ---
                break; // Found it, no need to read further
            }
        }
        cpuinfo_file.close();

        // --- DEBUGGING START ---
        if (frequency == 0.0f) {
            std::cout << "DEBUG: 'cpu MHz' line not found or frequency extraction failed." << std::endl;
        }
        // --- DEBUGGING END ---

    } else {
        std::cerr << "Error: Could not open /proc/cpuinfo to get CPU frequency." << std::endl;
        // --- DEBUGGING START ---
        std::cout << "DEBUG: Failed to open /proc/cpuinfo." << std::endl;
        // --- DEBUGGING END ---
    }

    return frequency;
}


