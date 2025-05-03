#include "../include/MemoryMonitor.h"

//Getting memory usage
size_t MemInfo::memUsage(int logger)
{

    std::string info = getInfo("/proc/meminfo");

    std::istringstream iss(info);
    std::string temp;
    std::vector<std::string> mem;
    while (iss >> temp)
    {
        mem.push_back(temp);
    }

    std::stringstream out;
    size_t memUsage = (stoi(mem[1]) - stoi(mem[4]));
    std::cout << "Free memory : " << mem[1] + mem[2] << " Available memory : " << mem[4] + mem[5] << " \x1b[41mMemory usage : " << memUsage << "kB\x1b[0m" << std::endl;
    if (logger == options::_NLOG)
    {
        out << "Free memory : " << mem[1] + mem[2] << " Available memory : " << mem[4] + mem[5] << " Memory usage : " << memUsage << "kB\n" << std::endl;
        log(out);
    }
    return memUsage;
}