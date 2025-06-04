#include "../include/CpuMonitor.h"
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

float CpuMonitor::getCpuFreq(){ 
    std:: ifstream cpuinfo_file("/proc/cpuinfo");
    std::string line;
    float frequency = 0.0f;
    if (cpuinfo_file.is_open()) {
        while (std::getline(cpuinfo_file, line)) {
            
            if (line.rfind("cpu MHz", 0) == 0) { 
                std::istringstream iss(line);
                std::string key;
                iss >> key; // Read "cpu"
                iss >> key; // Read "MHz"
                iss >> key; // Read ":"
                iss >> frequency; // Read the frequency value
                break; // Found it, no need to read further
            }
        }
        cpuinfo_file.close();
    } else {
        // Handle error: file could not be opened
        std::cerr << "Error: Could not open /proc/cpuinfo to get CPU frequency." << std::endl;
    }

    return frequency;
}

//daba ghade nchofo kifaach t extracti cpu freq mn system linux 

std::string CpuMonitor::getCpuInfo(){
    //To-Do
    return "";
}

