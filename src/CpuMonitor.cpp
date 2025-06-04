#include <iostream>
#include "../include/CpuMonitor.h"
#include <windows.h>


using namespace std;

int main() {
    CpuMonitor monitor;

    // Call update to refresh CPU stats
    if (monitor.update()) {
        CpuMonitor::CpuInfo info = monitor.getCpuInfo();
        cout << "Nombre de coeurs logiques : " << info.Nbrcores << endl;
        cout << "Frequence actuelle (MHz)  : " << info.frequency << endl;
        cout << "Frequence max (MHz)       : " << info.frequencyMax << endl;
        cout << "Utilisation CPU (%)       : " << info.usagePerCore << endl;
    } else {
        cout << "Echec de la mise a jour des informations CPU." << endl;
    }

    return 0;
}