#include <iostream>
#include "../include/MemoryMonitor.h"
#include "../include/CpuMonitor.h"

int main(int argc, char *argv[])
{

    // clear the screen after every run
    std::vector<std::string> options = {"--help", "--update", "--export"};
    // if (argc >= 2)
    // {
    //     for (auto param : options)
    //     {
    //         if (!strcmp(argv[1], "--help"))
    //             std::cout << "--help" << " Called\n";

    //         else if (!strcmp(argv[1], "--update"))
    //             std::cout << "--update" << " Called\n";

    //         else if (!strcmp(argv[1], "--export"))
    //             std::cout << "--export" << " Called\n";
    //         else
    //         {
    //             std::cout << argv[1] << " command not reconigzed";
    //         }
    //     }
    // }

    if (argc >= 2)
    {

        for (auto param : options)
        {
            char *argument = new char[param.length() + 1];
            strcpy(argument, param.c_str());
            if (!strcmp(argv[1], argument))
            {
                std::cout << param << " Called\n";
                break;
            }
            else
            {
                std::cout << "'" << argv[1] << "' command not reconigzed\n";
                exit(-1);
            }
        }
    }

    CpuMon cpu;

    while(true){
    cpu.calcCpuUsage();
    std::cout << "\x1b[2J\x1b[H]";}

    return 0;
}