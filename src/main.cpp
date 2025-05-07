#include <iostream>
#include "../include/CpuMonitor.h"

using namespace std;

int main(){


    // This is just a simple demo

    CpuMonitor::RefreshCpuMonitor();

    cout << "CPU :";
    for(unsigned long v : CpuMonitor::CPU){
        cout << " " << v;
    }
    cout << endl;

    for(int i=0; i<CpuMonitor::CORES_NUMBER; i++){
        cout << "CPU" << i << " :";
        for(unsigned long v : CpuMonitor::CORES[i]){
            cout << " " << v;
        }
        cout << endl;
    }

    cout << "INTERRUPTS : " << CpuMonitor::INTERRUPTS << endl;
    cout << "CONTEXT_SWITCHES : " << CpuMonitor::CONTEXT_SWITCHES << endl;
    cout << "BOOT_TIME : " << CpuMonitor::BOOT_TIME << endl;
    cout << "PROCESSES : " << CpuMonitor::PROCESSES << endl;
    cout << "PROCESSES_RUNNING : " << CpuMonitor::PROCESSES_RUNNING << endl;
    cout << "PROCESSES_BLOCKED : " << CpuMonitor::PROCESSES_BLOCKED << endl;
    cout << "SOFT_IRQS :";
    for(unsigned long v : CpuMonitor::SOFT_IRQS){
        cout << " " << v;
    }
    cout << endl;

    cout << "Example of CPU total user time : " << CpuMonitor::CPU[CpuMonitor::States::user] << endl;

    return 0;
}