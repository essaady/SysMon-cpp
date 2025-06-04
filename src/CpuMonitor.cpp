#include "../include/CpuMonitor.h"
using namespace std;

CpuMonitor::CpuMonitor() {
    CPU.frequencyMax = 0;
    CPU.nbrCPU = 0;  
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
    //To-Do
    // CPU.usageCPU  = getCpuUsage(); 
    // CPU.frequency = getCpuFreq(); 
    // rawCPU = getCpuInfo();
    return true;
}

float CpuMonitor::getCpuFreq(){
    //To-Do
    return 0.0;
}

std::string CpuMonitor::getCpuInfo(){
    //To-Do
    return "";
}

