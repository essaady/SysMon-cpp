include "ProcessMonitor.h"
#include <iostream>
#include <fstream> //donner  status a partir  proc/stat/ ..
#include <dirent.h> // lire le dossier /proc/
#include <sstream> //permet de manipuler facilement des cha�nes de caract�res et de convertir des donn�es.
#include <algorithm> //biblio pour le tri
using namespace std;

ProcessMonit::ProcessMonit() {
    readProcesses();
}
ProcessMonit::~ProcessMonit() {}
void ProcessMonit::readProcesses() {
    DIR* dir = opendir("/proc"); //contient tous les proce du syst
    if (dir != nullptr) {
        struct dirent* entry;//entry represent quelque dossier le lit par readdir()
        while ((entry = readdir(dir)) != nullptr) {  //no NULL
            int pid = atoi(entry->d_name); //atoi();fct qui convert string en int pour trouve PID d'un proce//
            //d_name variable definir dans struct dirent ; et struct dirent se trouve dans biblio <dirent.h>
            if (pid > 0) {
                Process proc;
                proc.id = pid;
                ifstream statFile("/proc/" + to_string(pid) + "/stat");//ouvrir le fichier qui contient les infos de proces
                include "ProcessMonitor.h"
                #include <iostream>
                #include <fstream> //donner  status a partir  proc/stat/ ..
                #include <dirent.h> // lire le dossier /proc/
                #include <sstream> //permet de manipuler facilement des cha�nes de caract�res et de convertir des donn�es.
                #include <algorithm> //biblio pour le tri
                using namespace std;
                
                ProcessMonit::ProcessMonit() {
                    readProcesses();
                }
                ProcessMonit::~ProcessMonit() {}
                void ProcessMonit::readProcesses() {
                    DIR* dir = opendir("/proc"); //contient tous les proce du syst
                    if (dir != nullptr) {
                        struct dirent* entry;//entry represent quelque dossier le lit par readdir()
                        while ((entry = readdir(dir)) != nullptr) {  //no NULL
                            int pid = atoi(entry->d_name); //atoi();fct qui convert string en int pour trouve PID d'un proce//
                            //d_name variable definir dans struct dirent ; et struct dirent se trouve dans biblio <dirent.h>
                            if (pid > 0) {
                                Process proc;
                                proc.id = pid;
                                ifstream statFile("/proc/" + to_string(pid) + "/stat");//ouvrir le fichier qui contient les infos de proces

                                if (line.find("VmRSS:") == 0) {
                                    proc.ramUsage = stol(line.substr(7)); // stol() convertire en long
                                    break;
                                }
                            }
                            ifstream cpuFile("/proc/stat");
                            string cpuLine;
                            getline(cpuFile, cpuLine);
                            istringstream cpuStream(cpuLine);
                            string cpuWord;
                            vector<long> cpuValues;
                            while (cpuStream >> cpuWord) { // prendre
                                if (isdigit(cpuWord[0])) cpuValues.push_back(stol(cpuWord));
                            }
                            long totalCpuTime = cpuValues[0] + cpuValues[1] + cpuValues[2] + cpuValues[3];
                    //Il convertit les valeurs de texte en nombres pour faciliter les calculs.  Il calcule le temps total utilis� (totalCpuTime)
                            processes.push_back(proc);// l'Ajoute de process dans vector
                        }
                    }
                    closedir(dir);
                }
            }
            vector<Process> ProcessMonit::getProcesses() const {
                return processes;
            }
            void ProcessMonit::displayProcesses() const {
                cout << "PID\t Name\t \tState\t  RAM (KB)\t CPU (%)\n";
                cout << "-------------------------------------------------------------\n";
                // le tri des procs a partir d'utilisation de cpu
                vector<Process> sortedProcesses = processes;
                sort(sortedProcesses.begin(), sortedProcesses.end(),
                    [](const Process& a, const Process& b) {
                         return a.cpuUsage > b.cpuUsage; // trie decroissat cpu
                    });
                for (const auto& p : sortedProcesses) {
                    cout << p.id << "\t" << p.name << "\t" << p.state << "\t"
                        << p.ramUsage << "\t\t" << p.cpuUsage << endl;
                }
            }
