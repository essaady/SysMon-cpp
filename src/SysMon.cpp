#include "SysMon.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <sys/utsname.h>

using namespace std;

int updateInterval = 1000; 
string exportPath = "system_monitor_data";
bool help = false;

static CpuMonitor* globalCpuMon = nullptr;
static MemoryMonitor* globalMemMon = nullptr;
static ProcessMonitor* globalProcMon = nullptr;

string exportAsText() {
    if (!globalCpuMon || !globalMemMon || !globalProcMon) {
        return "Erreur: Données de surveillance non initialisées";
    }
    
    stringstream ss;
    ss << "=== RAPPORT DE SURVEILLANCE SYSTÈME ===" << endl;
    ss << "Heure: " << getTime() << endl << endl;

    ss << "--- PROCESSEUR ---" << endl;
    ss << "Utilisation CPU: " << globalCpuMon->getCpuUsage() << "%" << endl;
    ss << "Fréquence: " << globalCpuMon->getCpuFreq() << " MHz" << endl;
    ss << "Nombre de cœurs: " << globalCpuMon->getNbrCPU() << endl;
    ss << globalCpuMon->getCpuInfo() << endl << endl;

    ss << "--- MÉMOIRE ---" << endl;
    ss << "Mémoire totale: " << globalMemMon->getTotalMemory() / (1024*1024) << " MB" << endl;
    ss << "Mémoire libre: " << globalMemMon->getFreeMemory() / (1024*1024) << " MB" << endl;
    ss << "Utilisation mémoire: " << globalMemMon->getMemoryUsagePercentage() << "%" << endl;
    ss << "Swap total: " << globalMemMon->getTotalSwap() / (1024*1024) << " MB" << endl;
    ss << "Utilisation swap: " << globalMemMon->getSwapUsagePercentage() << "%" << endl << endl;

    ss << "--- PROCESSUS ---" << endl;
    ss << "Nombre de processus: " << globalProcMon->getNbrProcess() << endl;
    ss << globalProcMon->getProcessInfo() << endl;
    
    return ss.str();
}

string exportAsCSV() {
    if (!globalCpuMon || !globalMemMon || !globalProcMon) {
        return "Erreur: Données de surveillance non initialisées";
    }
    
    stringstream ss;
    
    ss << "Timestamp,CPU_Usage,CPU_Freq,Total_Memory_MB,Free_Memory_MB,Memory_Usage_Percent,";
    ss << "Total_Swap_MB,Swap_Usage_Percent,Process_Count" << endl;
    
    ss << getTime() << ",";
    ss << globalCpuMon->getCpuUsage() << ",";
    ss << globalCpuMon->getCpuFreq() << ",";
    ss << globalMemMon->getTotalMemory() / (1024*1024) << ",";
    ss << globalMemMon->getFreeMemory() / (1024*1024) << ",";
    ss << globalMemMon->getMemoryUsagePercentage() << ",";
    ss << globalMemMon->getTotalSwap() / (1024*1024) << ",";
    ss << globalMemMon->getSwapUsagePercentage() << ",";
    ss << globalProcMon->getNbrProcess() << endl;
    
    return ss.str();
}

bool update(CpuMonitor cpuMon, MemoryMonitor memMon, ProcessMonitor procMon) {
    bool success = true;
    
    globalCpuMon = &cpuMon;
    globalMemMon = &memMon;
    globalProcMon = &procMon;
    
 
    if (!cpuMon.update()) {
        cerr << "Erreur lors de la mise à jour du moniteur CPU" << endl;
        success = false;
    }
    
    if (!memMon.update()) {
        cerr << "Erreur lors de la mise à jour du moniteur mémoire" << endl;
        success = false;
    }
    
    if (!procMon.update()) {
        cerr << "Erreur lors de la mise à jour du moniteur processus" << endl;
        success = false;
    }
    
    return success;
}

string getTime() {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);
    
    stringstream ss;
    ss << put_time(timeinfo, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

string getInfo() {
    struct utsname sysInfo;
    stringstream ss;
    
    if (uname(&sysInfo) == 0) {
        ss << "Système: " << sysInfo.sysname << endl;
        ss << "Version du noyau: " << sysInfo.release << endl;
        ss << "Version: " << sysInfo.version << endl;
        ss << "Architecture: " << sysInfo.machine << endl;
        ss << "Nom d'hôte: " << sysInfo.nodename << endl;
    } else {
        ss << "Impossible de récupérer les informations système" << endl;
    }
    
    return ss.str();
}
