#include "../include/CpuMonitor.h"
#include "../include/SysMon.h"
#include <unistd.h>
#include <cstdint>

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

// Getting cpu. snap in snapshot
uint64_t CpuMonitor::getSnap(std::string calc){

    uint64_t cpu;

    std::string temp = SysMon::getInfo("/proc/stat");
    std::istringstream iss(temp);
    iss >> temp;
    uint64_t value;
    std::vector<uint64_t> cpuInfo;
    while (iss >> value)
    {
        cpuInfo.push_back(value);
    }

    if (calc == "_USED")
    {
        cpu = cpuInfo[0] + cpuInfo[1] + cpuInfo[2] + cpuInfo[5] + cpuInfo[6] + cpuInfo[7];
    }
    else
    {
        for (auto temp : cpuInfo)
        {
            cpu += temp;
        }
    }

    return cpu;
}

float CpuMonitor::calcCpuUsage(int logger,int updateInterval)
{
    uint64_t cpu1 = getSnap("NULL");
    uint64_t notIdle1 = getSnap("_USED");
    usleep(updateInterval);
    uint64_t cpu2 = getSnap("NULL");
    uint64_t notIdle2 = getSnap("_USED");

    uint64_t TotalTime = cpu2 - cpu1;
    uint64_t UsedTime = notIdle2 - notIdle1;

    float results = ((float)UsedTime / (float)TotalTime) * 100;
    std::cout << "UsedTime : " << UsedTime << " TotalTime : " << TotalTime << " CpuUsage:  \x1b[41m" << results << "%\n\x1b[0m";
    

    //If the user wants to log
    if (logger == options::_NLOG)
    {
        std::stringstream out;
        out << SysMon::getTime() << "    UsedTime : " << UsedTime << " TotalTime : " << TotalTime << " CpuUsage: " << results << "%\n";

        SysMon::log(out);
    }
    return results;
}
