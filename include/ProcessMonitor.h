#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>

class ProcessMonitor
{
private:
    struct activeProcesses
    {
        DWORD pid;
        std::string name;
        float cpuUsage;
        SIZE_T memoryUsage; 
        std::string userName;
        std::string pathName;
        std::string startTime;
    };

    std::vector<activeProcesses> processes;

    std::string getUserNameFromHandle(HANDLE hProcess);
    std::string getProcessPath(DWORD pid);
    std::string getProcessStartTime(DWORD pid);

public:
    bool update();

    activeProcesses getProcess(int index); 
    std::string getProcessInfo();          
};
