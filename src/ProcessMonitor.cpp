#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <pdh.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "pdh.lib")
#elif __linux__
#include <unistd.h>
#include <sys/sysinfo.h>
#include <dirent.h>
#include <sys/times.h>
#endif

// ================== CLASSE PRINCIPALE SYSMON ==================
class SysMon {
private:
    int updateInterval;     // Intervalle de mise à jour
    bool fullLog;          // Log complet

public:
    SysMon(int interval = 2000, bool log = false) 
        : updateInterval(interval), fullLog(log) {}

    // Fonctions principales
    int run();
    std::string exportAsText();
    std::string exportAsCSV();
    bool update();
    std::string getTime();
    std::string getInfo(const std::string& type);
    void log();

    // Setters
    void setUpdateInterval(int interval) { updateInterval = interval; }
    void setFullLog(bool log) { fullLog = log; }
};

// ================== MODULE CPU MONITOR ==================
class CpuMonitor {
private:
    struct CPU {
        float frequency;
        float frequencyMax;
        float usageCPU;
        float* usagePerCPU;
        short nbrCPU;
    } cpu;
    
    std::string rawCPU;

#ifdef _WIN32
    PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;
    std::vector<PDH_HCOUNTER> cpuCores;
#elif __linux__
    struct CpuTimes {
        long long user, nice, system, idle, iowait, irq, softirq;
    };
    std::vector<CpuTimes> prevCpuTimes;
#endif

public:
    CpuMonitor();
    ~CpuMonitor();

    bool update();
    float getCpuUsage();
    float getCpuFreq();
    std::string getCpuInfo();
    short getNbrCPU() const { return cpu.nbrCPU; }
    float getUsagePerCPU(int core) const;

private:
    void initializeCpuMonitoring();
    bool readCpuStats();
};

// ================== MODULE MEMORY MONITOR ==================
class MemoryMonitor {
private:
    struct RAM {
        size_t totalMemInMb;
        float usage;
        float freeMem;
        size_t swapMemInMb;
        float usageSwp;
        float freeSwp;
    } ram;
    
    std::vector<std::string> memInfo;

public:
    MemoryMonitor();

    bool update();
    unsigned long getTotalMemory();
    unsigned long getFreeMemory();
    unsigned long getUsedMemory();
    double getMemoryUsagePercentage();
    unsigned long getTotalSwap();
    unsigned long getFreeSwap();
    unsigned long getUsedSwap();
    double getSwapUsagePercentage();

private:
    void readMemoryInfo();
    size_t parseMemoryValue(const std::string& line);
};

// ================== MODULE PROCESS MONITOR ==================
class ProcessMonitor {
private:
    struct ActiveProcesses {
        float cpu;
        float memory;
        struct {
            std::string user;
            std::string pathName;
        } time;
    } AP;
    
    int* PID;
    int nbrProcess;

    struct ProcessInfo {
        unsigned long pid;
        std::string name;
        std::string user;
        std::string pathName;
        double cpuUsage;
        size_t memoryUsage;
        size_t virtualMemory;
        std::string status;
        long long cpuTime;
        std::chrono::steady_clock::time_point lastUpdateTime;
    };

    std::vector<ProcessInfo> processes;
    std::map<unsigned long, ProcessInfo> previousData;

public:
    ProcessMonitor();
    ~ProcessMonitor();

    bool update();
    ActiveProcesses getProcess(int index);
    std::string getProcessInfo();
    std::string getProcessRaw();
    int getNbrProcess() const { return nbrProcess; }
    void findProcess(const std::string& name);
    void killProcess(int pid);

private:
    bool getProcessList();
    void calculateCpuUsage(ProcessInfo& info);
    std::string formatBytes(size_t bytes);

#ifdef _WIN32
    bool getProcessListWindows();
#elif __linux__
    bool getProcessListLinux();
    bool getProcessInfoLinux(ProcessInfo& info);
#endif
};

// ================== IMPLÉMENTATION CPU MONITOR ==================
CpuMonitor::CpuMonitor() {
    cpu.nbrCPU = 0;
    cpu.usagePerCPU = nullptr;
    initializeCpuMonitoring();
}

CpuMonitor::~CpuMonitor() {
    delete[] cpu.usagePerCPU;
#ifdef _WIN32
    PdhCloseQuery(cpuQuery);
#endif
}

void CpuMonitor::initializeCpuMonitoring() {
#ifdef _WIN32
    PdhOpenQuery(NULL, NULL, &cpuQuery);
    PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    cpu.nbrCPU = sysInfo.dwNumberOfProcessors;
    
    cpu.usagePerCPU = new float[cpu.nbrCPU];
    cpuCores.resize(cpu.nbrCPU);
    
    for (int i = 0; i < cpu.nbrCPU; i++) {
        std::wstring counterPath = L"\\Processor(" + std::to_wstring(i) + L")\\% Processor Time";
        PdhAddEnglishCounter(cpuQuery, counterPath.c_str(), NULL, &cpuCores[i]);
    }
    
    PdhCollectQueryData(cpuQuery);
    
#elif __linux__
    cpu.nbrCPU = sysconf(_SC_NPROCESSORS_ONLN);
    cpu.usagePerCPU = new float[cpu.nbrCPU];
    prevCpuTimes.resize(cpu.nbrCPU + 1); // +1 pour le total
#endif
}

bool CpuMonitor::update() {
    return readCpuStats();
}

bool CpuMonitor::readCpuStats() {
#ifdef _WIN32
    PdhCollectQueryData(cpuQuery);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    PdhCollectQueryData(cpuQuery);
    
    PDH_FMT_COUNTERVALUE counterVal;
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    cpu.usageCPU = static_cast<float>(counterVal.dblValue);
    
    for (int i = 0; i < cpu.nbrCPU; i++) {
        PdhGetFormattedCounterValue(cpuCores[i], PDH_FMT_DOUBLE, NULL, &counterVal);
        cpu.usagePerCPU[i] = static_cast<float>(counterVal.dblValue);
    }
    
    return true;
    
#elif __linux__
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::ifstream stat("/proc/stat");
    
    if (!stat.is_open()) return false;
    
    std::string line;
    int cpuIndex = 0;
    
    while (std::getline(stat, line) && cpuIndex <= cpu.nbrCPU) {
        if (line.substr(0, 3) == "cpu") {
            std::istringstream iss(line);
            std::string cpuLabel;
            iss >> cpuLabel;
            
            CpuTimes times;
            iss >> times.user >> times.nice >> times.system >> times.idle 
                >> times.iowait >> times.irq >> times.softirq;
            
            if (cpuIndex < prevCpuTimes.size()) {
                long long prevIdle = prevCpuTimes[cpuIndex].idle + prevCpuTimes[cpuIndex].iowait;
                long long idle = times.idle + times.iowait;
                
                long long prevNonIdle = prevCpuTimes[cpuIndex].user + prevCpuTimes[cpuIndex].nice +
                                       prevCpuTimes[cpuIndex].system + prevCpuTimes[cpuIndex].irq +
                                       prevCpuTimes[cpuIndex].softirq;
                long long nonIdle = times.user + times.nice + times.system + times.irq + times.softirq;
                
                long long prevTotal = prevIdle + prevNonIdle;
                long long total = idle + nonIdle;
                
                long long totalDiff = total - prevTotal;
                long long idleDiff = idle - prevIdle;
                
                if (totalDiff > 0) {
                    float usage = (float)(totalDiff - idleDiff) * 100.0f / totalDiff;
                    if (cpuIndex == 0) {
                        cpu.usageCPU = usage;
                    } else if (cpuIndex - 1 < cpu.nbrCPU) {
                        cpu.usagePerCPU[cpuIndex - 1] = usage;
                    }
                }
            }
            
            prevCpuTimes[cpuIndex] = times;
            cpuIndex++;
        }
    }
    
    return true;
#endif
}

float CpuMonitor::getCpuUsage() {
    return cpu.usageCPU;
}

float CpuMonitor::getCpuFreq() {
    return cpu.frequency;
}

std::string CpuMonitor::getCpuInfo() {
    std::ostringstream oss;
    oss << "CPU Usage: " << std::fixed << std::setprecision(1) << cpu.usageCPU << "%\n";
    oss << "Number of cores: " << cpu.nbrCPU << "\n";
    
    for (int i = 0; i < cpu.nbrCPU; i++) {
        oss << "Core " << i << ": " << std::fixed << std::setprecision(1) 
            << cpu.usagePerCPU[i] << "%\n";
    }
    
    return oss.str();
}

float CpuMonitor::getUsagePerCPU(int core) const {
    if (core >= 0 && core < cpu.nbrCPU) {
        return cpu.usagePerCPU[core];
    }
    return 0.0f;
}

// ================== IMPLÉMENTATION MEMORY MONITOR ==================
MemoryMonitor::MemoryMonitor() {
    memset(&ram, 0, sizeof(ram));
}

bool MemoryMonitor::update() {
    readMemoryInfo();
    return true;
}

void MemoryMonitor::readMemoryInfo() {
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    
    if (GlobalMemoryStatusEx(&memInfo)) {
        ram.totalMemInMb = memInfo.ullTotalPhys / (1024 * 1024);
        size_t freeMemBytes = memInfo.ullAvailPhys;
        ram.freeMem = freeMemBytes / (1024.0f * 1024.0f);
        ram.usage = ((memInfo.ullTotalPhys - memInfo.ullAvailPhys) * 100.0f) / memInfo.ullTotalPhys;
        
        ram.swapMemInMb = memInfo.ullTotalPageFile / (1024 * 1024);
        size_t freeSwapBytes = memInfo.ullAvailPageFile;
        ram.freeSwp = freeSwapBytes / (1024.0f * 1024.0f);
        ram.usageSwp = ((memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) * 100.0f) / memInfo.ullTotalPageFile;
    }
    
#elif __linux__
    struct sysinfo si;
    if (sysinfo(&si) == 0) {
        ram.totalMemInMb = (si.totalram * si.mem_unit) / (1024 * 1024);
        ram.freeMem = (si.freeram * si.mem_unit) / (1024.0f * 1024.0f);
        ram.usage = ((si.totalram - si.freeram) * 100.0f) / si.totalram;
        
        ram.swapMemInMb = (si.totalswap * si.mem_unit) / (1024 * 1024);
        ram.freeSwp = (si.freeswap * si.mem_unit) / (1024.0f * 1024.0f);
        if (si.totalswap > 0) {
            ram.usageSwp = ((si.totalswap - si.freeswap) * 100.0f) / si.totalswap;
        }
    }
#endif
}

unsigned long MemoryMonitor::getTotalMemory() {
    return ram.totalMemInMb * 1024 * 1024;
}

unsigned long MemoryMonitor::getFreeMemory() {
    return static_cast<unsigned long>(ram.freeMem * 1024 * 1024);
}

unsigned long MemoryMonitor::getUsedMemory() {
    return getTotalMemory() - getFreeMemory();
}

double MemoryMonitor::getMemoryUsagePercentage() {
    return ram.usage;
}

unsigned long MemoryMonitor::getTotalSwap() {
    return ram.swapMemInMb * 1024 * 1024;
}

unsigned long MemoryMonitor::getFreeSwap() {
    return static_cast<unsigned long>(ram.freeSwp * 1024 * 1024);
}

unsigned long MemoryMonitor::getUsedSwap() {
    return getTotalSwap() - getFreeSwap();
}

double MemoryMonitor::getSwapUsagePercentage() {
    return ram.usageSwp;
}

// ================== IMPLÉMENTATION PROCESS MONITOR ==================
ProcessMonitor::ProcessMonitor() {
    nbrProcess = 0;
    PID = nullptr;
}

ProcessMonitor::~ProcessMonitor() {
    delete[] PID;
}

bool ProcessMonitor::update() {
    return getProcessList();
}

std::string ProcessMonitor::formatBytes(size_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
    return oss.str();
}

bool ProcessMonitor::getProcessList() {
    processes.clear();
    
#ifdef _WIN32
    return getProcessListWindows();
#elif __linux__
    return getProcessListLinux();
#endif
    return false;
}

#ifdef _WIN32
bool ProcessMonitor::getProcessListWindows() {
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) return false;
    
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return false;
    }
    
    do {
        ProcessInfo info;
        info.pid = pe32.th32ProcessID;
        info.name = pe32.szExeFile;
        info.status = "Running";
        info.user = "System";
        
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
                                    FALSE, pe32.th32ProcessID);
        if (hProcess) {
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                info.memoryUsage = pmc.WorkingSetSize;
                info.virtualMemory = pmc.PagefileUsage;
            }
            
            FILETIME creationTime, exitTime, kernelTime, userTime;
            if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) {
                ULARGE_INTEGER kt, ut;
                kt.LowPart = kernelTime.dwLowDateTime;
                kt.HighPart = kernelTime.dwHighDateTime;
                ut.LowPart = userTime.dwLowDateTime;
                ut.HighPart = userTime.dwHighDateTime;
                info.cpuTime = kt.QuadPart + ut.QuadPart;
            }
            
            CloseHandle(hProcess);
        }
        
        calculateCpuUsage(info);
        processes.push_back(info);
        
    } while (Process32Next(hProcessSnap, &pe32));
    
    CloseHandle(hProcessSnap);
    nbrProcess = processes.size();
    return true;
}
#endif

#ifdef __linux__
bool ProcessMonitor::getProcessListLinux() {
    DIR* procDir = opendir("/proc");
    if (!procDir) return false;
    
    struct dirent* entry;
    while ((entry = readdir(procDir)) != nullptr) {
        if (strspn(entry->d_name, "0123456789") == strlen(entry->d_name)) {
            ProcessInfo info;
            info.pid = std::stoul(entry->d_name);
            
            if (getProcessInfoLinux(info)) {
                calculateCpuUsage(info);
                processes.push_back(info);
            }
        }
    }
    
    closedir(procDir);
    nbrProcess = processes.size();
    return true;
}

bool ProcessMonitor::getProcessInfoLinux(ProcessInfo& info) {
    std::string statPath = "/proc/" + std::to_string(info.pid) + "/stat";
    std::string statusPath = "/proc/" + std::to_string(info.pid) + "/status";
    
    std::ifstream statFile(statPath);
    if (!statFile.is_open()) return false;
    
    std::string line;
    std::getline(statFile, line);
    std::istringstream iss(line);
    
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.size() < 24) return false;
    
    info.name = tokens[1];
    if (info.name.front() == '(' && info.name.back() == ')') {
        info.name = info.name.substr(1, info.name.length() - 2);
    }
    
    info.status = tokens[2];
    
    long long utime = std::stoll(tokens[13]);
    long long stime = std::stoll(tokens[14]);
    info.cpuTime = utime + stime;
    
    info.virtualMemory = std::stoull(tokens[22]);
    info.memoryUsage = std::stoull(tokens[23]) * getpagesize();
    
    info.user = "user"; // Simplification
    info.pathName = "/proc/" + std::to_string(info.pid) + "/exe";
    
    return true;
}
#endif

void ProcessMonitor::calculateCpuUsage(ProcessInfo& info) {
    auto currentTime = std::chrono::steady_clock::now();
    info.cpuUsage = 0.0;
    
    if (previousData.find(info.pid) != previousData.end()) {
        ProcessInfo& prev = previousData[info.pid];
        
        auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>
                       (currentTime - prev.lastUpdateTime).count();
        
        if (timeDiff > 0) {
            long long cpuTimeDiff = info.cpuTime - prev.cpuTime;
            
#ifdef _WIN32
            info.cpuUsage = (cpuTimeDiff * 100.0) / (timeDiff * 10000.0);
#elif __linux__
            long int clockTicks = sysconf(_SC_CLK_TCK);
            info.cpuUsage = (cpuTimeDiff * 100.0) / (clockTicks * timeDiff / 1000.0);
#endif
            if (info.cpuUsage > 100.0) info.cpuUsage = 100.0;
            if (info.cpuUsage < 0.0) info.cpuUsage = 0.0;
        }
    }
    
    info.lastUpdateTime = currentTime;
    previousData[info.pid] = info;
}

ProcessMonitor::ActiveProcesses ProcessMonitor::getProcess(int index) {
    if (index >= 0 && index < processes.size()) {
        AP.cpu = processes[index].cpuUsage;
        AP.memory = (processes[index].memoryUsage / (1024.0f * 1024.0f));
        AP.time.user = processes[index].user;
        AP.time.pathName = processes[index].pathName;
    }
    return AP;
}

std::string ProcessMonitor::getProcessInfo() {
    std::ostringstream oss;
    oss << "Total Processes: " << nbrProcess << "\n";
    oss << "Active Processes: " << processes.size() << "\n\n";
    
    // Trier par usage CPU
    std::sort(processes.begin(), processes.end(),
             [](const ProcessInfo& a, const ProcessInfo& b) {
                 return a.cpuUsage > b.cpuUsage;
             });
    
    oss << std::left << std::setw(8) << "PID"
        << std::setw(20) << "Name"
        << std::setw(8) << "CPU%"
        << std::setw(12) << "Memory" << "\n";
    oss << std::string(50, '-') << "\n";
    
    int count = std::min(10, static_cast<int>(processes.size()));
    for (int i = 0; i < count; i++) {
        const auto& proc = processes[i];
        oss << std::left << std::setw(8) << proc.pid
            << std::setw(20) << proc.name.substr(0, 19)
            << std::setw(8) << std::fixed << std::setprecision(1) << proc.cpuUsage
            << std::setw(12) << formatBytes(proc.memoryUsage) << "\n";
    }
    
    return oss.str();
}

std::string ProcessMonitor::getProcessRaw() {
    std::ostringstream oss;
    for (const auto& proc : processes) {
        oss << proc.pid << "," << proc.name << "," << proc.cpuUsage 
            << "," << proc.memoryUsage << "\n";
    }
    return oss.str();
}

// ================== IMPLÉMENTATION SYSMON PRINCIPALE ==================
int SysMon::run() {
    CpuMonitor cpuMon;
    MemoryMonitor memMon;
    ProcessMonitor procMon;
    
    std::cout << "=== SysMon-cpp - Surveillance Système ===" << std::endl;
    std::cout << "Intervalle de mise à jour: " << updateInterval << "ms" << std::endl;
    std::cout << "Appuyez sur Ctrl+C pour arrêter..." << std::endl;
    
    while (true) {
        // Effacer écran
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
        
        std::cout << "=== SysMon-cpp - " << getTime() << " ===" << std::endl;
        
        // Mise à jour des modules
        cpuMon.update();
        memMon.update();
        procMon.update();
        
        // Affichage CPU
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "MODULE CPU MONITOR" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << cpuMon.getCpuInfo() << std::endl;
        
        // Affichage Mémoire
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "MODULE MEMORY MONITOR" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "Total Memory: " << memMon.getTotalMemory() / (1024*1024) << " MB" << std::endl;
        std::cout << "Used Memory: " << memMon.getUsedMemory() / (1024*1024) << " MB" << std::endl;
        std::cout << "Memory Usage: " << std::fixed << std::setprecision(1) 
                  << memMon.getMemoryUsagePercentage() << "%" << std::endl;
        std::cout << "Swap Usage: " << std::fixed << std::setprecision(1) 
                  << memMon.getSwapUsagePercentage() << "%" << std::endl;
        
        // Affichage Processus
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "MODULE PROCESS MONITOR" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << procMon.getProcessInfo() << std::endl;
        
        if (fullLog) {
            log();
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(updateInterval));
    }
    
    return 0;
}

bool SysMon::update() {
    return true;
}

std::string SysMon::getTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&time_t);
    timeStr.pop_back(); // Enlever \n
    return timeStr;
}

std::string SysMon::getInfo(const std::string& type) {
    return "Info: " + type;
}

void SysMon::log() {
    if (fullLog) {
        std::ofstream logFile("sysmon.log", std::ios::app);
        if (logFile.is_open()) {
            logFile << getTime() << " - System monitoring update" << std::endl;
            logFile.close();
        }
    }
}

std::string SysMon::exportAsText() {
    std::ostringstream oss;
    oss << "=== SysMon-cpp Report ===" << std::endl;
    oss << "Generated: " << getTime() << std::endl;
    oss << "Update Interval: " << updateInterval << "ms" << std::endl;
    return oss.str();
}

std::string SysMon::exportAsCSV() {
    std::ostringstream oss;
    oss << "Timestamp,Module,Value" << std::endl;
    oss << getTime() << ",System,Active" << std::endl;
    return oss.str();
}

// ================== FONCTION MAIN ==================
int main() {
    try {
        SysMon monitor(2000, true);
        return monitor.run();
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }
}
