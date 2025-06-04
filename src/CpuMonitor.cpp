#include "../include/CpuMonitor.h"
#include <windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <winreg.h>


ULONGLONG CpuMonitor::subtractTimes(const FILETIME& a, const FILETIME& b) {
    ULONGLONG t1 = (static_cast<ULONGLONG>(a.dwHighDateTime) << 32) | a.dwLowDateTime;
    ULONGLONG t2 = (static_cast<ULONGLONG>(b.dwHighDateTime) << 32) | b.dwLowDateTime;
    return t1 - t2;
}


bool CpuMonitor::update() {
    usageCPU = getCpuUsage();
    frequency = getCpuFreq();
    frequencyMax = frequency; 
    nbrCPU = std::thread::hardware_concurrency();
    rawCPU = getCpuInfo();
    return true;
}


float CpuMonitor::getCpuUsage() {
    FILETIME idle1, kernel1, user1;
    FILETIME idle2, kernel2, user2;

    if (!GetSystemTimes(&idle1, &kernel1, &user1))
        return 0.0f;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (!GetSystemTimes(&idle2, &kernel2, &user2))
        return 0.0f;

    ULONGLONG idleDiff = subtractTimes(idle2, idle1);
    ULONGLONG kernelDiff = subtractTimes(kernel2, kernel1);
    ULONGLONG userDiff = subtractTimes(user2, user1);

    ULONGLONG total = kernelDiff + userDiff;
    if (total == 0) return 0.0f;

    return (100.0f * (total - idleDiff)) / total;
}


float CpuMonitor::getCpuFreq() {
    HKEY hKey;
    DWORD data = 0;
    DWORD dataSize = sizeof(DWORD);

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        RegQueryValueExA(hKey, "~MHz", NULL, NULL, reinterpret_cast<LPBYTE>(&data), &dataSize);
        RegCloseHandle(hKey);
    }

    return static_cast<float>(data);
}


std::string CpuMonitor::getCpuInfo() {
    HKEY hKey;
    CHAR buffer[256];
    DWORD bufferSize = sizeof(buffer);
    std::ostringstream oss;

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
        "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        if (RegQueryValueExA(hKey, "ProcessorNameString", NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
            oss << "Processor: " << buffer << "\n";
        }

        DWORD mhz = 0;
        bufferSize = sizeof(mhz);
        if (RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&mhz, &bufferSize) == ERROR_SUCCESS) {
            oss << "Frequency: " << mhz << " MHz\n";
        }

        RegCloseHandle(hKey);
    }

    oss << "Cores: " << std::thread::hardware_concurrency() << "\n";
    return oss.str();
}
