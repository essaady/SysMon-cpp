#include "..\include\SysMon.h"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <fstream>

SysMon::SysMon(int interval, bool log) : updateInterval(interval), fullLog(log) {}

bool SysMon::update() {
    bool cpuOk = cpuMonitor.update();
    bool memOk = memoryMonitor.update();
    bool procOk = processMonitor.update();
    return cpuOk && memOk && procOk;
}

std::string SysMon::getTime() {
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));
    return std::string(buf);
}

std::string SysMon::getInfo(const std::string& type) {
    if (type == "CPU") return cpuMonitor.getCpuInfo();
    else if (type == "RAM") return std::to_string(memoryMonitor.getUsedMemory());
    else if (type == "PROC") return processMonitor.getProcessInfo();
    return "";
}

std::string SysMon::exportAsText() {
    std::ostringstream oss;
    oss << "[ " << getTime() << " ]\n\n";
    oss << ">>> CPU <<<\n";
    oss << "Utilisation : " << cpuMonitor.getCpuUsage() << " %\n";
    oss << "Fréquence   : " << cpuMonitor.getCpuFreq() << " MHz\n\n";

    oss << ">>> MÉMOIRE <<<\n";
    oss << "Total : " << memoryMonitor.getTotalMemory() << " MB\n";
    oss << "Utilisée : " << memoryMonitor.getUsedMemory() << " MB (" 
        << memoryMonitor.getMemoryUsagePercentage() << " %)\n";
    oss << "Swap utilisée : " << memoryMonitor.getUsedSwap() << " MB\n\n";

    oss << ">>> PROCESSUS ACTIFS <<<\n";
    oss << processMonitor.getProcessInfo();

    return oss.str();
}

std::string SysMon::exportAsCSV() {
    std::ostringstream oss;
    oss << "Time,CPU_Usage,CPU_Freq,Total_Mem,Used_Mem,Swap_Used\n";
    oss << getTime() << ","
        << cpuMonitor.getCpuUsage() << ","
        << cpuMonitor.getCpuFreq() << ","
        << memoryMonitor.getTotalMemory() << ","
        << memoryMonitor.getUsedMemory() << ","
        << memoryMonitor.getUsedSwap() << "\n";

    return oss.str();
}

void SysMon::log() {
    std::ofstream out("sysmon.log", std::ios::app);
    if (out.is_open()) {
        out << exportAsText() << "\n";
        out.close();
    }
}

int SysMon::run() {
    while (true) {
        update();
        std::cout << exportAsText() << std::endl;
        if (fullLog) log();
        std::this_thread::sleep_for(std::chrono::seconds(updateInterval));
    }
    return 0;
}
