#include "MemoryMonitor.h"
#include <fstream>      // Pour lire les fichiers
#include <sstream>      // Pour analyser les lignes
#include <iostream>
#include <iomanip>      // Pour formater les nombres 
using namespace std;

void MemoryMonitor::update() {
    ifstream file("/proc/meminfo");
    string line;
    totalMemory = 0;
    availableMemory = 0;

    while (getline(file, line)) {  //Boucle qui lit le fichier ligne par ligne
        istringstream iss(line);
        string key;
        long value;
        string unit;

        iss >> key >> value >> unit;

        if (key == "MemTotal:") {
            totalMemory = value;  
        } else if (key == "MemAvailable:") {
            availableMemory = value;  
        }

        if (totalMemory > 0 && availableMemory > 0)
            break;
    }
}

string MemoryMonitor::getUsageInfo() {
    long usedMemory = totalMemory - availableMemory;

    double usedGB = usedMemory / 1024.0 / 1024.0;
    double totalGB = totalMemory / 1024.0 / 1024.0;

    ostringstream output;
    output << fixed << setprecision(1);  
    output << "RAM Used: " << usedGB << " GB / " << totalGB << " GB";

    return output.str();
}
