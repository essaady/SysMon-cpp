#include <iostream>
#include "ProcessMonitor.h"
using namespace std;

int main() {
    ProcessMonitor pm;

    pm.update();

    activeProcesses p = pm.getProcess(0);

    cout << "Infos format texte: " << pm.getProcessInfo() << endl;
    cout << "Infos brutes: " << pm.getProcessRaw() << endl;

    return 0;
}

// Fin de la Tache 