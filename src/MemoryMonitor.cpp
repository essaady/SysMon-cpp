#include "../include/MemoryMonitor.h"
#include "../include/SysMon.h"
using namespace std;

MemoryMonitor::MemoryMonitor() {
    // Initialisation des ressources si nécessaire
}

MemoryMonitor::~MemoryMonitor() {
    // Libération des ressources si nécessaire

}

// Getting memory usage
size_t MemoryMonitor::memUsage(int logger)
{
    std::string info = SysMon::getInfo("/proc/meminfo");

    std::istringstream iss(info);
    std::string temp;
    std::vector<std::string> mem;
    while (iss >> temp)
    {
        mem.push_back(temp);
    }

    size_t memUsage = (stoi(mem[1]) - stoi(mem[4]));
    std::cout << "Free memory : " << mem[1] + mem[2] << " Available memory : " << mem[4] + mem[5] << " \x1b[41mMemory usage : " << memUsage << "kB\x1b[0m" << std::endl;
    if (logger == options::_NLOG)
    {
        std::stringstream out;

        out << "Free memory : " << mem[1] + mem[2] << " Available memory : " << mem[4] + mem[5] << " Memory usage : " << memUsage << "kB\n"
            << std::endl;
        SysMon::log(out);
    }
    return memUsage;
}

bool MemoryMonitor::update(){
    //To-Do
    memUsage(0);
    return true;
}
