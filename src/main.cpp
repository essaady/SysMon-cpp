#include "../include/FileUtils.h"
#include "../include/MemoryMonitor.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<string> lignes = FileUtils::readFileLines("/proc/cpuinfo");

    for (const string& ligne : lignes) {
        cout << ligne << endl;
    }

    float memUsage = MemoryMonitor::getMemoryUsage();
    cout << "Memory Usage: " << memUsage << "%" << endl;

    return 0;
}
