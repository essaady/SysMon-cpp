#ifndef SYSMON_H
#define SYSMON_H

#include <string>
using namespace std;

//? Variables globales utilisées un peu partout

// Contrôle la fréquence de mise à jour des données système (en secondes ou millisecondes)
extern int updateInterval;

// Définit le chemin du fichier ou le format d'export des données collectées
extern string exportPath;

// Indique s'il faut afficher l'aide (true) ou exécuter la surveillance (false)
extern bool help;

//? Déclarations anticipées des classes de monitoring
class CpuMonitor;
class MemoryMonitor;
class ProcessMonitor;

// Classe principale de supervision
class SysMon {
public:
    // Met à jour les données CPU, mémoire et processus avec les instances fournies
    // Retourne true si la mise à jour a réussi, false sinon
    bool update(CpuMonitor cpuMon, MemoryMonitor memMon, ProcessMonitor procMon);

    // Retourne les données de supervision formatées en texte brut (lisible)
    string exportAsText() const;

    // Retourne les données de supervision formatées au format CSV (valeurs séparées par des virgules)
    string exportAsCSV() const;

    // Retourne l'heure actuelle du système sous forme de chaîne
    string getTime() const;

    // Retourne des informations générales sur la machine (OS, uptime, etc.)
    string getInfo() const;
};

#endif // SYSMON_H
