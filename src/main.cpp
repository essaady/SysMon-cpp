#include <iostream>
#include "../include/CpuMonitor.h"

using namespace std;

int main(){


    // This is just a simple demo

    CpuMonitor CPU;
    system("sleep 10");

    CPU.update();



    cout << "CPU Frequency : " << CPU.CPU.frequency << "GHz\n";
    cout << "CPU Frequency Max : " << CPU.CPU.frequencyMax << "GHz\n";
    cout << "Cores Number : " << CPU.CPU.nbrCPU << endl;
    cout << "CPU usage : " << CPU.CPU.usageCPU << "%\n";
    cout << "cores usage : \n";
    for(int i=0; i<CPU.CPU.nbrCPU; i++){
        cout << "Core " << i << " : " << CPU.CPU.usagePerCpu[i] << "%\n";
    }
    cout << "Informations in single line : " << CPU.rawCPU << endl;
    return 0;
}