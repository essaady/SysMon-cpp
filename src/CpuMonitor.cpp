#include "../include/CpuMonitor.h"
#include <fstream> //gestion des fichiers (on utilise ifstream)
#include <sstream> //manipuler les flux de chaines de caracteres (istringstream et ostringstream)
#include <thread> //pour utiliser std::this_thread::sleep_for()
#include <chrono> //pour utiliser chrono dans sleep_for
#include <iostream>

using namespace std;

bool CpuMonitor::update() {
    //lecture
    ifstream stat1("/proc/stat"); //ifstream pour ouvrir le fichier "/proc/stat" en lecture
    if (!stat1.is_open()) { //verifier si le fichier a bien ete ouvert
        cout << "erreur: impossible d'ouvrir /proc/stat" << endl;
        return false;
    }

    string line;
    getline(stat1, line); //lire 1er ligne (cpu global)
    stat1.close(); //fermer le fichier quand la lecture terminee

    //extraire les valeurs de la ligne
    istringstream iss1(line); //analyser et decouper la ligne pour extraire facilement les mots (flux d'entree depuis une chaine de caracteres)
    string cpu_label;
    long long user1, nice1, system1, idle1;
    iss1 >> cpu_label >> user1 >> nice1 >> system1 >> idle1; //iss1 lit depuis la ligne et extrait le 1er mot dans cpu_label et les valeurs numirics d'apres dans user1, nice1, system1, idle1
    long long total1 = user1 + nice1 + system1 + idle1;

    //attendre 1s pour 2eme mesure
    this_thread::sleep_for(chrono::seconds(1)); //c la fct std::this_thread::sleep_for pour une pause de 1s, et chrono pour la gestion du temps (documentation deja mentionner dans main.cpp, memoryMonitor)

    //2eme lecture
    ifstream stat2("/proc/stat");
    if (!stat2.is_open()) {
        cout << "erreur: impossible d'ouvrir /proc/stat" << endl;
        return false;
    }

    getline(stat2, line);
    stat2.close();

    //extraire les valeurs
    istringstream iss2(line);
    string cpu_labe2;
    long long user2, nice2, system2, idle2;
    iss2 >> cpu_labe2 >> user2 >> nice2 >> system2 >> idle2;
    long long total2 = user2 + nice2 + system2 + idle2;

    //calculer les differences
    long long total_diff = total2 - total1;
    long long idle_diff = idle2 - idle1;

    //calculer le pourcentage d'utilisation
    if (total_diff > 0) {
        CPU.usageCPU = ((float)(total_diff - idle_diff) / total_diff) * 100.0f; 
        //(float) est un casting avant la division
        //suffixe f (dans 100.0f) force le type float, sans f c'est un double par defaut (32bits vs 64bits)
    } else {
        CPU.usageCPU = 0.0f; //initialise usageCPU a 0 en float
    }

    return true;
}

float CpuMonitor::getCpuUsage() const {
    return CPU.usageCPU; //retourne le pourcentage d'utilisation
}

float CpuMonitor::getCpuFreq() const {
    //lecture de la frequence actuelle depuis /sys
    ifstream cpuinfo("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    if (!cpuinfo.is_open()) {
        return 0.0f; //retourne 0 si impossible de lire
    }

    float freq;
    cpuinfo >> freq;
    return freq / 1000.0f; //convertir kHz en MHz
}

string CpuMonitor::getCpuInfo() const {
    //creer une chaine avec les infos cpu
    ostringstream oss; //construire et formater une chaine de caracteres en sortie (flux de sortie vers une chaine de caracteres)
    oss << "CPU Use: " << CPU.usageCPU << "% | Frequency: " << getCpuFreq() << " MHz";
    return oss.str(); //convertit le contenu du flux oss en string et le retourne (fct str() de ostringstream)
}
