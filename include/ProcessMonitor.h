#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <string>
using namespace std;

// structure pour stocker les infos d un processus
struct activeProcesses {
    float cpu;
    float memory;
    string user;
    string pathName;
};

class ProcessMonitor {
private:
    int* PID; // tableau des PID
    int nbrProcess; // nombre total de processus
    activeProcesses AP; // un processus actif (pour simplifier)

public:
    ProcessMonitor(); // constructeur
    ~ProcessMonitor(); // destructeur
    bool update(); // mettre a jour les infos
    activeProcesses getProcess(int index); // retourner un processus
    string getProcessInfo(); // infos lisibles
    string getProcessRaw();  // infos brutes
};

#endif // Fin de fichier ProcessMonitor.h