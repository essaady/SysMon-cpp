#include "../include/CpuMonitor.h"

std::string CpuMon::getCpuInfo()
{
    std::string content;
    std::stringstream content_stream;

    std::fstream info("/proc/stat", std::ios::in);
    if(!info){
        std::cerr << "There was an error opening the file" << std::endl;
        exit(-1);
    }
    //std::getline(info, content);
    content_stream << info.rdbuf();
    content = content_stream.str();
    return content;
}

//Methode 1
// uint64_t CpuMon::getSnap(std::string calc)
// {
//     uint64_t cpu = 0;
//     int counter = 5;
//     unsigned short space = 0;
//     std::string cpuInfo = getCpuInfo();
    
//     char current_char;
//     while (current_char != '\n')
//     {
//         current_char = cpuInfo[counter];
//         if (isdigit(current_char))
//         {
//             buffer.push_back(current_char);
//         }
//         else if (isspace(current_char))
//         {
//             space++;
//             if ((space == 4 || space == 5) && calc == "_USED")
//             {
//                 counter++;
//                 buffer.clear();
//                 continue;
//             }
//             cpu += stoi(buffer);
//             buffer.clear();
//         }
//         counter++;
//     }
//     return cpu;
// }

//Method 2
uint64_t CpuMon::getSnap(std::string calc){

    uint64_t cpu;

    std::string temp = getCpuInfo();
    std::istringstream iss(temp);
    iss >> temp;
    uint64_t value;
    std::vector<uint64_t> cpuInfo;
    while(iss >> value){
        cpuInfo.push_back(value);
    }

    if(calc == "_USED"){
        cpu = cpuInfo[0] + cpuInfo[1] + cpuInfo[2] + cpuInfo[5] + cpuInfo[6] + cpuInfo[7];
    }else{
        for(auto temp : cpuInfo){
            cpu += temp;
        }
    }

    return cpu;
}


float CpuMon::calcCpuUsage()
{
    uint64_t cpu1 = getSnap("NULL");
    uint64_t notIdle1 = getSnap("_USED");
    usleep(1000000);
    uint64_t cpu2 = getSnap("NULL");
    uint64_t notIdle2 = getSnap("_USED");

    uint64_t TotalTime = cpu2 - cpu1;
    uint64_t UsedTime = notIdle2 - notIdle1;

    float results = ((float)UsedTime / (float)TotalTime * 100);
    std::cout << "UsedTime : " << UsedTime << " TotalTime : " << TotalTime << " CpuUsage:  "<< results << "%\n";
    return results;
}