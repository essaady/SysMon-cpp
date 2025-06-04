#include "../include/SysMon.h"
#include "../include/CpuMonitor.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>

int main(int argc, char *argv[])
{
    // clear the screen on start
    system("clear");

    std::vector<std::string> options = {"--help", "--update", "--export", "--cpu"};
    int option = 0;
    bool showCpuOnly = false;

    // checking for user argument
    if (argc >= 2) {
        bool isRecognized = false;

        for (const auto& param : options) {
            if (param == argv[1]) {
                std::cout << param << " Called\n";
                isRecognized = true;

                if (param == "--export") {
                    option = options::_NLOG;
                } else if (param == "--cpu") {
                    showCpuOnly = true;
                }

                break;
            }
        }

        if (!isRecognized) {
            std::cout << "'" << argv[1] << "' command not recognized\n";
            return -1;
        }
    }

    if (showCpuOnly) {
        CpuMonitor monitor;

        while (true) {
            float usage = monitor.getCpuUsage();
            if (usage < 0) {
                std::cerr << "Erreur lecture CPU\n";
                return 1;
            }

            std::cout << "Utilisation CPU : " << usage << " %" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    int updateInterval = 500000; // 0.5 sec in microseconds
    SysMon SysMonCpp(updateInterval);

    return SysMonCpp.run();
}
