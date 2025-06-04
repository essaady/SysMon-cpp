#include <iostream>
#include "../include/MemoryMonitor.h"
#include "../include/CpuMonitor.h"
#include "../include/ProcessMonitor.h"



int main(int argc, char *argv[])
{

    // clear the screen on start
    system("clear");
    std::vector<std::string> options = {"--help", "--export"};
    int option = 0;
    // checking for user argument
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
        else if(!strcmp(argv[1], "--help")){
            std::cout << "./SysMon\nA program that will calculate both the memory and cpu usage, while also showing the current running processes.\noptions:\n--help : show commandes\n--export: export mem and cpu usage in a log file.\n";
            exit(0);
        }
        
    }

    CpuMon cpu;
    MemInfo mem;
    ProcMon process;
    // Calculating and printing both cpu and memory usage
    int i = 0;
    while (i < 10)
    {
        system("clear");
        cpu.calcCpuUsage(option);
        mem.memUsage(option);
        process.getProcess();
        i++;
        usleep(500000);
    }

    
    return 0;
}