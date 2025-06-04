#include "../include/SysMon.h"


int main(int argc, char *argv[])
{

    // clear the screen on start
    system("clear");
    vector<string> options = {"--help", "--update", "--export"};
    // checking for user argument
    int option = 0;
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
    
    int updateInterval = 5e5;
    cout << "#########################################################################"<<endl;

    // SysMon SysMonCpp(updateInterval);
    // SysMonCpp.update();
    // float cpuFreq = SysMonCpp.getCpuFreq();
    // std::cout << "Current CPU Frequency: " << cpuFreq << " MHz" << std::endl;

    CpuMonitor testCpuMonitor;
    testCpuMonitor.update();
    float cpuFfreq = testCpuMonitor.getCpuFreq();
    std:: cout<< "current CPU freq :" << cpuFfreq << " mhz"<<std::endl;

    SysMon SysMonCpp(updateInterval);

    return SysMonCpp.run();
}