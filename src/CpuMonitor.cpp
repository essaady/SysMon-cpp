#include "Cpu.h"
#include <windows.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <string>
#include <vector>

#pragma comment(lib, "pdh.lib")

Cpu::Cpu() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    cpu.coreCount = sysInfo.dwNumberOfProcessors;
    cpu.usagePerCore = new float[cpu.coreCount];

    cpu.frequency = 0.0f;
    cpu.frequencyMax = 0.0f;
    cpu.usage = 0.0f;
    rawInfo = "Unknown Processor";
}

Cpu::~Cpu() {
    delete[] cpu.usagePerCore;
}

bool Cpu::update() {

    HKEY hKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                    "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 
                    0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD mhz;
        DWORD size = sizeof(mhz);
        if (RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE)&mhz, &size) == ERROR_SUCCESS) {
            cpu.frequency = static_cast<float>(mhz) / 1000.0f; 
            cpu.frequencyMax = cpu.frequency * 1.2f; 
        }
        RegCloseKey(hKey);
    }


    static PDH_HQUERY cpuQuery;
    static std::vector<PDH_HCOUNTER> coreCounters(cpu.coreCount);
    static bool firstRun = true;

    if (firstRun) {
        PdhOpenQuery(NULL, 0, &cpuQuery);
        for (short i = 0; i < cpu.coreCount; i++) {
            std::string counterPath = "\\Processor(" + std::to_string(i) + ")\\% Processor Time";
            PdhAddEnglishCounterA(cpuQuery, counterPath.c_str(), 0, &coreCounters[i]);
        }
        PdhCollectQueryData(cpuQuery);
        firstRun = false;
    }

    if (PdhCollectQueryData(cpuQuery) == ERROR_SUCCESS) {
        PDH_FMT_COUNTERVALUE counterVal;
        cpu.usage = 0.0f;
        
        for (short i = 0; i < cpu.coreCount; i++) {
            if (PdhGetFormattedCounterValue(coreCounters[i], PDH_FMT_DOUBLE, NULL, &counterVal) == ERROR_SUCCESS) {
                cpu.usagePerCore[i] = static_cast<float>(counterVal.doubleValue);
                cpu.usage += cpu.usagePerCore[i];
            }
        }
        cpu.usage /= cpu.coreCount; 
    }


    HKEY cpuKey;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                    "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 
                    0, KEY_READ, &cpuKey) == ERROR_SUCCESS) {
        char processorName[256];
        DWORD size = sizeof(processorName);
        if (RegQueryValueEx(cpuKey, "ProcessorNameString", NULL, NULL, 
                          (LPBYTE)processorName, &size) == ERROR_SUCCESS) {
            rawInfo = processorName;
        }
        RegCloseKey(cpuKey);
    }

    return true;
}
