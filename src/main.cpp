#include "SysMon.h"

int updateInterval = 1;
std::string exportFormat = "text";
bool help = false;

std::string exportAsText() { return ""; }
std::string exportAsCSV() { return ""; }

bool update(CpuMonitor& cpu, MemoryMonitor& mem, ProcessMonitor& proc) {
    return cpu.update() && mem.update() && proc.update();
}

std::string getTime() { return ""; }
std::string getInfo() { return ""; }

int main() {
    CpuMonitor cpu;
    MemoryMonitor mem;
    ProcessMonitor proc;
    update(cpu, mem, proc);
    return 0;
}
