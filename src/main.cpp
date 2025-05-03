#include <iostream>
#include "../include/MemoryMonitor.h"
#include "../include/CpuMonitor.h"

int main(int argc, char *argv[])
{

    // clear the screen on start
    system("clear");
    std::vector<std::string> options = {"--help", "--update", "--export"};
    int option = 0;
    //checking for user argument
    if (argc >= 2)
    {
        bool isReconized = false;
        for (auto param : options)
        {
            char *argument = new char[param.length() + 1];
            strcpy(argument, param.c_str());
            if (!strcmp(argv[1], argument))
            {
                std::cout << param << " Called\n";
                isReconized = true;
                break;
            }
        }
        if (!isReconized)
        {
            std::cout << "'" << argv[1] << "' command not reconigzed\n";
            exit(-1);
        }

        if (!strcmp(argv[1], "--export"))
        {
            option = options::_NLOG;
        }
    }

    CpuMon cpu;
    MemInfo mem;
    //Calculating and printing both cpu and memory usage
    while (true)
    {
        cpu.calcCpuUsage(option);
        mem.memUsage(option);
        std::cout << "\x1b[2J\x1b[H";
    }

    return 0;
}