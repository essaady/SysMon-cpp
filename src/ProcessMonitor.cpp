#include "../include/ProcessMonitor.h"
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <sddl.h>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "advapi32.lib")



bool ProcessMonitor::update() {
    AP.clear();

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return false;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        CloseHandle(hSnapshot);
        return false;
    }

    do {
        ProcessMonitor::activeProcesses proc;
        proc.cpu = 0.0f; 
        proc.time = "N/A";

        DWORD pid = pe32.th32ProcessID;
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

        if (hProcess) {
            TCHAR exePath[MAX_PATH];
            if (GetModuleFileNameEx(hProcess, NULL, exePath, MAX_PATH)) {
                proc.pathName = exePath;
            } else {
                proc.pathName = "Unknown";
            }

            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                proc.memory = pmc.WorkingSetSize / (1024.0f * 1024.0f); // en MB
            } else {
                proc.memory = 0.0f;
            }

            HANDLE hToken;
            if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                DWORD size = 0;
                GetTokenInformation(hToken, TokenUser, NULL, 0, &size);
                PTOKEN_USER tokenUser = (PTOKEN_USER)malloc(size);
                if (GetTokenInformation(hToken, TokenUser, tokenUser, size, &size)) {
                    LPSTR name = NULL;
                    ConvertSidToStringSidA(tokenUser->User.Sid, &name);
                    proc.user = name ? std::string(name) : "unknown";
                    LocalFree(name);
                } else {
                    proc.user = "unknown";
                }
                free(tokenUser);
                CloseHandle(hToken);
            } else {
                proc.user = "unknown";
            }

            CloseHandle(hProcess);
        } else {
            proc.pathName = "Access denied";
            proc.memory = 0.0f;
            proc.user = "unknown";
        }

        AP.push_back(proc);

    } while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);
    nbrProcess = AP.size();
    return true;
}

ProcessMonitor::activeProcesses ProcessMonitor::getProcess(int index) {
    if (index < 0 || index >= (int)AP.size()) {
        return {};
    }
    return AP[index];
}

std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream oss;
    int index = 0;
    for (const auto& proc : AP) {
        oss << "Process " << index++ << ":\n";
        oss << "  User: " << proc.user << "\n";
        oss << "  CPU Usage: " << proc.cpu << " s\n";
        oss << "  Memory: " << proc.memory << " MB\n";
        oss << "  Path: " << proc.pathName << "\n";
        oss << "  Time: " << proc.time << "\n\n";
    }
    return oss.str();
}

std::string ProcessMonitor::getProcessRaw() {
    std::ostringstream oss;
    for (const auto& proc : AP) {
        oss << proc.user << "," << proc.cpu << "," << proc.memory << "," << proc.pathName << "," << proc.time << "\n";
    }
    return oss.str();
}
