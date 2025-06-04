#include "../include/ProcessMonitor.h"
#include <psapi.h>
#include <sstream>
#include <iomanip>

bool ProcessMonitor::update()
{
    processes.clear();

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
        return false;

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshot, &pe))
    {
        CloseHandle(snapshot);
        return false;
    }

    do
    {
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe.th32ProcessID);
        if (hProcess)
        {
            activeProcesses proc;
            proc.pid = pe.th32ProcessID;
            proc.name = pe.szExeFile;
            proc.userName = getUserNameFromHandle(hProcess);
            proc.pathName = getProcessPath(pe.th32ProcessID);
            proc.startTime = getProcessStartTime(pe.th32ProcessID);

            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
            {
                proc.memoryUsage = pmc.WorkingSetSize;
            }
            else
            {
                proc.memoryUsage = 0;
            }

            proc.cpuUsage = 0.0f;

            processes.push_back(proc);
            CloseHandle(hProcess);
        }

    } while (Process32Next(snapshot, &pe));

    CloseHandle(snapshot);
    return true;
}

ProcessMonitor::activeProcesses ProcessMonitor::getProcess(int index)
{
    if (index < 0 || index >= static_cast<int>(processes.size()))
    {

        for (const auto &proc : processes)
        {
            if (static_cast<int>(proc.pid) == index)
                return proc;
        }
        throw std::out_of_range("Index or PID not found");
    }

    return processes[index];
}

std::string ProcessMonitor::getProcessInfo()
{
    std::ostringstream oss;
    oss << std::left << std::setw(6) << "PID"
        << std::setw(25) << "Name"
        << std::setw(10) << "CPU(%)"
        << std::setw(12) << "Memory(KB)"
        << std::setw(20) << "User"
        << std::setw(30) << "Path"
        << "StartTime" << "\n";

    for (const auto &proc : processes)
    {
        oss << std::left << std::setw(6) << proc.pid
            << std::setw(25) << proc.name
            << std::setw(10) << std::fixed << std::setprecision(2) << proc.cpuUsage
            << std::setw(12) << proc.memoryUsage / 1024
            << std::setw(20) << proc.userName
            << std::setw(30) << proc.pathName
            << proc.startTime << "\n";;
    }

    return oss.str();
}
