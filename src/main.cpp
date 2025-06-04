#include "../include/MemoryMonitor.h"
#include <iostream>
using namespace std;

int main() {
    MemoryMonitor monitor;
    if (monitor.update()) {
        cout << "Memory usage: " << monitor.getMemoryUsagePercentage() << "%" << endl;
        cout << "Swap usage: " << monitor.getSwapUsagePercentage() << "%" << endl;
    } else {
        cout << "Erreur lors de la lecture de /proc/meminfo" << endl;
    }

    return 0;
}
