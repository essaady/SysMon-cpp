#include "../include/CpuMonitor.h"
#include <iostream>
#include <string>

#ifdef __linux__

#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <csting>

CpuMonitor::CpuMonitor() {
      std::ifstream file("/proc/stat");
      if (!file.is_open()) throw std::runtime_error("Erreur durant l'ouverture du fichier !")
      std::string line;
      cpu.nbrCPU = 0;
      while (std::getline(file, line)) {
          if (line.rfind("cpu", 0) == 0 && line[3] != ' ')
              cpu.nbrCPU++;
          cpu.usagePerCPU = new float[cpu.nbrCPU];
          std::memset(cpu.usagePerCPU, 0, sizeof(float) * cpu.nbrCPU);
      }
}

bool CpuMonitor::update(int choix) {
    try {
        // CPU Usage
        if (choix == 0) {
     std::vector<unsigned long long> idle1(cpu.nbrCPU), total1(cpu.nbrCPU);
     std::vector<unsigned long long> idle2(cpu.nbrCPU), total2(cpu.nbrCPU);
     
     auto linuxStats = [&](std::vector<unsigned long long> &idle, std::vector<unsigned long long> &total) {
          std::ifstream file("/proc/stat");
          if (!file.is_open()) return false
          std::string line;
          int i = 0;

          while (std::getline(file, line)) {
               if (std::rfind("cpu", 0) == 0 && line[3] != ' ') {
                   std::istringstream ss(line);
                   unsigned long long user, nice, system, idleVal, iowait, irq, softirq, steal;
                   ss >> label >> user >> nice >> system >> idleVal >> iowait >> irq >> softirq >> steal;
                   
                   if (ss.fail()) return false;

                   unsigned long long idleTime = idleVal + iowait;
                   unsigned long long totalTime = idleTime + user + nice + system + irq + softirq + steal;
                   if (index < cpu.nbrCPU) {
                     idle[index] = idleTime;
                     total[index] = totalTime;
                     ++index; 
                   }
                   
               }
          }
          if (index != cpu.nbrCPU) return false
     };

     linuxStats(idle1, total1);
     std::this_thread::sleep_for(std::chrono::milliseconds(100));
     linuxStats(idle2, total2);
     for (int i=0; i < cpu.nbrCPU; ++i) {
        unsigned long long D_IDLE = idle2[i] - idle1[i];
        unsigned long long D_TOTAL = total2[i] - total1[i];
        if (D_TOTAL == 0) {
             cpu.usagePerCPU[i] = 0.0f;
        } 
        else {
            cpu.usagePerCPU[i] = 100.0f * (D_TOTAL - D_IDLE) / D_TOTAL;
        }
     }
    }
     // Frequency
    else if (choix == 1) { 
    std::ifstream cpuinfo("/proc/cpuinfo");
    if (!cpuinfo.is_open()) return false;

    std::string line_;
    float freqSum = 0;
    int count = 0;

    while (std::getline(cpuinfo, line)) {
        if (line.find("cpu MHz") != std::string::npos) {
            size_t colon = line.find(':');
            if (colon != std::string::npos) {
                std::string freqStr = line.substr(colon + 1);
                try {
                    float freq = std::stof(freqStr);
                    freqSum += freq;
                    count++;
                } catch (...) {
                    return false;
                }
            }
        }
    }

    if (count == 0) return false;
    cpu.frequency = freqSum / count;
    
    // Max Frequence
    std::ifstream maxFreq("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq");
    if (!maxFreq.is_open()) return false;

    unsigned long freq_khz;
    maxFreq >> freq_khz;
    maxFreq.close();

    cpu.frequencyMax = static_cast<float>(freq_khz) / 1000.0f;
      }
    return true;
    }
    catch (...) {
        return false;
    }
     
}
#endif

#ifdef _WIN32
#include <windows.h>
#include <winternl.h>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>

typedef LONG (WINAPI *NtQuerySystemInformationFunc)(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

typedef struct _SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION {
    LARGE_INTEGER IdleTime;          
    LARGE_INTEGER KernelTime;        
    LARGE_INTEGER UserTime;          
    LARGE_INTEGER DpcTime;           
    LARGE_INTEGER InterruptTime;    
    ULONG InterruptCount;            
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION, *PSYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;


CpuMonitor::CpuMonitor() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    cpu.nbrCPU = static_cast<short>(sysInfo.dwNumberOfProcessors);

    cpu.usagePerCPU = new float[cpu.nbrCPU];
    std::memset(cpu.usagePerCPU, 0, sizeof(float) * cpu.nbrCPU);
}

bool CpuMonitor::update(int choix) {
    try {
    // CPU Usage
     if (choix == 0) {
    static std::vector<SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION> statsPrev;
    static bool firstRun = true;

    HMODULE hNtDll = LoadLibraryA("ntdll.dll");
    if (!hNtDll) return false;

    auto NtQuerySystemInformation =
        reinterpret_cast<NtQuerySystemInformationFunc>(
            GetProcAddress(hNtDll, "NtQuerySystemInformation"));
    if (!NtQuerySystemInformation) {
        FreeLibrary(hNtDll);
        return false;
    }

    ULONG len = sizeof(SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION) * cpu.nbrCPU;
    std::vector<SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION> statsCurr(cpu.nbrCPU);

    if (NtQuerySystemInformation(SystemProcessorPerformanceInformation, statsCurr.data(), len, nullptr) != 0) {
        FreeLibrary(hNtDll);
        return false;
    }

    if (firstRun) {
        statsPrev = statsCurr;
        firstRun = false;
        FreeLibrary(hNtDll);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return update();
    }

    for (int i = 0; i < cpu.nbrCPU; ++i) {
        auto& prev = statsPrev[i];
        auto& curr = statsCurr[i];

        ULONGLONG idleDiff = curr.IdleTime.QuadPart - prev.IdleTime.QuadPart;
        ULONGLONG sysDiff = (curr.KernelTime.QuadPart + curr.UserTime.QuadPart) -
                           (prev.KernelTime.QuadPart + prev.UserTime.QuadPart);

        ULONGLONG total = sysDiff + idleDiff;
        if (total == 0) {
            cpu.usagePerCPU[i] = 0.0f;
        } else {
            cpu.usagePerCPU[i] = 100.0f * (total - idleDiff) / total;
        }
    }

    statsPrev = statsCurr;
    FreeLibrary(hNtDll);
    }

    // Frequency 
    else if (choix == 1) {
    HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) return false;

    hres = CoInitializeSecurity(NULL, -1, NULL, NULL,
                               RPC_C_AUTHN_LEVEL_DEFAULT,
                               RPC_C_IMP_LEVEL_IMPERSONATE,
                               NULL, EOAC_NONE, NULL);
    if (FAILED(hres)) {
        CoUninitialize();
        return false;
    }

    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                           IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hres)) {
        CoUninitialize();
        return false;
    }

    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
    if (FAILED(hres)) {
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                            RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
                            NULL, EOAC_NONE);
    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT CurrentClockSpeed FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL, &pEnumerator);
    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return false;
    }

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;
    float freq = -1;

    if (pEnumerator) {
        if (pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn) == S_OK) {
            VARIANT vtProp;
            pclsObj->Get(L"CurrentClockSpeed", 0, &vtProp, 0, 0);
            freq = static_cast<float>(vtProp.intVal);
            VariantClear(&vtProp);
            pclsObj->Release();
        }
        pEnumerator->Release();
    }

    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    cpu.frequency = freq;

    // Max Frequence
    HRESULT hres_ = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres_)) return false;

    CoInitializeSecurity(NULL, -1, NULL, NULL,
                         RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
                         NULL, EOAC_NONE, NULL);

    IWbemLocator* pLoc = NULL;
    CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER,
                     IID_IWbemLocator, (LPVOID*)&pLoc);

    IWbemServices* pSvc = NULL;
    pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);

    CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                      RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
                      NULL, EOAC_NONE);

    IEnumWbemClassObject* pEnumerator = NULL;
    pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t("SELECT MaxClockSpeed FROM Win32_Processor"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL, &pEnumerator);

    IWbemClassObject* pclsObj = NULL;
    ULONG uReturn = 0;
    float maxFreq = -1;

    if (pEnumerator && pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn) == S_OK) {
        VARIANT vtProp;
        pclsObj->Get(L"MaxClockSpeed", 0, &vtProp, 0, 0);
        maxFreq = static_cast<float>(vtProp.intVal);
        VariantClear(&vtProp);
        pclsObj->Release();
    }

    if (pEnumerator) pEnumerator->Release();
    if (pSvc) pSvc->Release();
    if (pLoc) pLoc->Release();
    CoUninitialize();

    cpu.frequencyMax =  maxFreq;
    }
    return true;
   }
   catch (...) {
    return false;
   }
}

#endif


float CpuMonitor::getCpuUsage() {
      float cpuUsage = 0.0f;
      update(0);
      for (int i = 0; i < cpu.nbrCPU; ++i)
          cpuUsage += cpu.usagePerCPU[i];

      return cpuUsage / cpu.nbrCPU;
}

float CpuMonitor::getCpuFreq() {
      float freq = 0.0f;
      update(1);
      return cpu.frequency;
}

std::string CpuMonitor::getCpuInfo() {
    return "[+] CPU cores: " + std::to_string(cpu.nbrCPU) + 
           "\n[+] CPU usage: " + std::to_string(cpu.usageCPU) + "%" + 
           "\n[+] CPU Frequency: " + std::to_string(cpu.frequency) + "MHz" + 
           "\n[+] CPU Max Frequency: " + std::to_string(cpu.frequencyMax) + "MHz\n";
}

CpuMonitor::~CpuMonitor() {
    delete[] cpu.usagePerCPU;
}

