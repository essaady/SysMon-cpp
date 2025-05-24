#ifndef PROCESS_MONIT_H
#define PROCESS_MONIT_H

#include <vector> // Biblio pour stocker les infos dynamiquement (on peut utiliser un tableau, mais si le nombre de processus dépasse la taille, cela pose problème)
#include <string>
using namespace std;
struct Process {
    int id;            // Numéro du processus unique!
    string name;
    string state;// État du processus
    long uptime; //temps d'exécution en secondes
    long ramUsage;
    double cpuUsage;
};

class ProcessMonit {
private:
    vector<Process> processes; // stocker les infos du proces
    void readProcesses();

public:
    ProcessMonit();
    ~ProcessMonit();
    vector<Process> getProcesses() const;
    void displayProcesses() const;
};

#endif // PROCESS_MONIT_H
