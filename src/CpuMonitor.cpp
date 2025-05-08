#include "../include/CpuMonitor.h"

// Getting cpu. snap in snapshot
uint64_t CpuMon::getSnap(std::string calc)
{

    uint64_t cpu;

    std::string temp = getInfo("/proc/stat");
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

float CpuMon::calcCpuUsage(int logger)
{
    uint64_t cpu1 = getSnap("NULL");
    uint64_t notIdle1 = getSnap("_USED");
    usleep(500000);
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
        out << getTime() << "    UsedTime : " << UsedTime << " TotalTime : " << TotalTime << " CpuUsage: " << results << "%\n";

        log(out);
    }
    return results;
}
