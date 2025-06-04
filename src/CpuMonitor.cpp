#include "../include/MemoryMonitor.h"
#include <iostream>
#include <fstream> //gestion des fichiers (on utilise ifstream)
#include <sstream> //manipuler les flux de chaines de caracteres (istringstream et ostringstream)

using namespace std;

bool MemoryMonitor::update() {
    ifstream meminfo("/proc/meminfo"); //ifstream pour ouvrir le fichier "/proc/meminfo" en lecture
    if (!meminfo.is_open()) { //verifier si le fichier a bien ete ouvert
        cout << "Erreur: Impossible d'ouvrir /proc/meminfo" << endl;
        return false;
    }

    //vider le vecteur memInfo avant de le remplir
    memInfo.clear();
    
    string line;
    unsigned long total_memory = 0, free_memory = 0, total_swap = 0, free_swap = 0;
    
    while (getline(meminfo, line)) { //lire le fichier ligne par ligne
        memInfo.push_back(line); //stocker chaque ligne dans le vecteur
        
        istringstream iss(line); //analyser et decouper la ligne pour extraire facilement les mots (flux d'entree depuis une chaine de caracteres)
        string key;
        unsigned long value;

        //lecture de la cle et de la valeur
        iss >> key >> value; //iss lit depuis la ligne et extrait le premier mot dans key et le second (valeur numerique) dans value

        //extraction des informations necessaires
        if (key == "MemTotal:") {
            total_memory = value;
        } else if (key == "MemFree:") {
            free_memory = value;
        } else if (key == "SwapTotal:") {
            total_swap = value;
        } else if (key == "SwapFree:") {
            free_swap = value;
        }
    }

    //mettre a jour la structure RAM
    RAM.totalMemInMb = total_memory / 1024; //convertir de KB a MB
    RAM.freeMem = static_cast<float>(free_memory) / 1024.0f; //convertir de KB a MB
    // static_cast est un operateur pour faire le casting
    //suffixe f (dans 1024.0f) indique que la constante est de type float, sans f le compilateur utilise double par defaut (32bits vs 64bits)
    RAM.usage = static_cast<float>(total_memory - free_memory) / 1024.0f; //memoire utilisee en MB
    
    RAM.SwapMemInMb = total_swap / 1024; //convertir de KB a MB
    RAM.freeSwp = static_cast<float>(free_swap) / 1024.0f; //convertir de KB a MB
    RAM.usageSwp = static_cast<float>(total_swap - free_swap) / 1024.0f; //swap utilisee en MB

    meminfo.close(); //fermer le fichier quand la lecture terminee
    return true;
}

unsigned long MemoryMonitor::getTotalMemory() {
    return RAM.totalMemInMb * 1024; //en KB
}

unsigned long MemoryMonitor::getFreeMemory() {
    return static_cast<unsigned long>(RAM.freeMem * 1024); //en KB
}

unsigned long MemoryMonitor::getUsedMemory() {
    return static_cast<unsigned long>(RAM.usage * 1024); //en KB
}

double MemoryMonitor::getMemoryUsagePercentage() {
    if (RAM.totalMemInMb == 0) return 0.0;
    return (RAM.usage / RAM.totalMemInMb) * 100.0;
}

unsigned long MemoryMonitor::getTotalSwap() {
    return RAM.SwapMemInMb * 1024; //en KB
}

unsigned long MemoryMonitor::getFreeSwap() {
    return static_cast<unsigned long>(RAM.freeSwp * 1024); //en KB
}

unsigned long MemoryMonitor::getUsedSwap() {
    return static_cast<unsigned long>(RAM.usageSwp * 1024); //en KB
}

double MemoryMonitor::getSwapUsagePercentage() {
    if (RAM.SwapMemInMb == 0) return 0.0;
    return (RAM.usageSwp / RAM.SwapMemInMb) * 100.0;
}
