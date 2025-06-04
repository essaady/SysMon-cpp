#include <iostream>
#include <unistd.h> // for usleep
#include <cstring>
#include "../include/MemoryMonitor.h"
#include "../include/CpuMonitor.h"
#include "../include/ProcessMonitor.h"

int main(int argc, char *argv[])
{
    // clear the screen on start
    system("clear");
    std::vector<std::string> options = {"--help", "--update", "--export"};
    int option = 0;

    // checking for user argument
    if (argc >= 2)
    {
        bool isRecognized = false;
        for (auto &param : options)
        {
            if (param == argv[1])
            {
                std::cout << param << " Called\n";
                isRecognized = true;
                break;
            }
        }
        if (!isRecognized)
        {
            std::cout << "'" << argv[1] << "' command not recognized\n";
            exit(-1);
        }

        if (!strcmp(argv[1], "--export"))
        {
            option = options::_NLOG; // Assuming this is defined elsewhere
        }
    }

    CpuMon cpu;
    MemoryMonitor mem;
    ProcMon process;

    int i = 0;
    while (i < 10)
    {
        usleep(1000000);
        system("clear");

        cpu.calcCpuUsage(option);

        if (mem.update())
        {
            std::cout << "Memory Usage: " << mem.getMemoryUsagePercentage() << "%" << std::endl;
            std::cout << "Swap Usage: " << mem.getSwapUsagePercentage() << "%" << std::endl;
        }
        else
        {
            std::cout << "Erreur lors de la lecture de /proc/meminfo" << std::endl;
        }

        process.getProcess();

        i++;
    }

    return 0;
}
