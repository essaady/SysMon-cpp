#include "../include/ProcessMonitor.h"

std::vector<std::string> getVector(std::istringstream &iss)
{
    int i = 0;
    std::vector<std::string> info;
    std::string content;
    while (iss >> content && i < 13)
    {
        info.push_back(content);
        i++;
    }

    return info;
}

bool isNumber(std::string &string)
{
    bool isNumber = true;
    for (auto str : string)
    {
        if (!std::isdigit(str))
        {
            isNumber = false;
            break;
        }
    }
    return isNumber;
}

void ProcMon::getProcess()
{
    std::cout << "\x1b[42m Name       " << "PID        \x1b[0m\n";
    std::string path = "/proc";
    int processCount = 0;
    for (auto const &entry : std::filesystem::directory_iterator(path))
    {
        std::string newPath = entry.path();
        std::string pathFileName = entry.path().filename();
        if (isNumber(pathFileName))
        {
            std::string fileContent = getInfo(newPath + "/status");
            if(fileContent == " "){
                continue;
            }
            std::istringstream iss(fileContent);

            std::vector<std::string> info = getVector(iss);
            processCount++;

            std::cout << info[1] << "    " << info[12] << std::endl;
        }
    }


    std::cout << "There are : " << processCount << " processes\n";
}