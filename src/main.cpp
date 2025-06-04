#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

void printHeader() {
    std::cout << std::left
              << std::setw(20) << "CPU Usage (%)"
              << std::setw(20) << "Memory Used (MB)"
              << std::setw(20) << "Processes Count"
              << std::endl;

    std::cout << std::string(60, '=') << std::endl;
}

void printData(float cpuUsage, double memUsed, int procCount) {
    std::cout << std::left
              << std::setw(20) << cpuUsage
              << std::setw(20) << memUsed
              << std::setw(20) << procCount
              << std::endl;
}

int main() {
    while (true) {
        float cpuUsage = 35.7f;    
        double memUsed = 4200.5;
        int procCount = 123;

        std::cout << "\033[2J\033[H";  

        printHeader();
        printData(cpuUsage, memUsed, procCount);

        std::this_thread::sleep_for(std::chrono::seconds(2));

    }

    return 0;
}
