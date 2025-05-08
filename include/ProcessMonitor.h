#ifndef PROCESS_MONIT_H
#define PROCESS_MONIT_H

#include <vector> // Biblio pour stocker les infos dynamiquement (on peut utiliser un tableau, mais si le nombre de processus d�passe la taille, cela pose probl�me)
#include <string>
using namespace std;
struct Process {
    int id;            // Num�ro du processus unique!
    string name;
    string state;// �tat du processus
    long uptime; //temps d'ex�cution en secondes
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