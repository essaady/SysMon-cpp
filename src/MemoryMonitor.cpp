#include "../include/MemoryMonitor.h"
#include <iostream>
#include <fstream> //gestion des fichiers (on utilise ifstream)
#include <sstream> //manipuler les flux de chaines de caracteres (istringstream et ostringstream)
#include <format> // pour std::format

using namespace std;

//initialisation des valeurs a 0 et mise a jour
MemoryMonitor::MemoryMonitor() : total_memory(0), free_memory(0) {
    update();
}

bool MemoryMonitor::readMemoryInfo() {
    ifstream meminfo("/proc/meminfo"); //ifstream pour ouvrir le fichier "/proc/meminfo" en lecture
    if (!meminfo.is_open()) { //verifier si le fichier a bien ete ouvert
        cout << "Erreur: Impossible d'ouvrir /proc/meminfo" << endl;
        return false;
    }

    string line;
    while (getline(meminfo, line)) { //lire le fichier ligne par ligne
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
        }
    }

    meminfo.close(); //fermer le fichier quand la lecture terminee
    return true;
}

bool MemoryMonitor::update() {
    return readMemoryInfo();
}

unsigned long MemoryMonitor::getTotalMemory() const {
    return total_memory;
}

unsigned long MemoryMonitor::getFreeMemory() const {
    return free_memory;
}

unsigned long MemoryMonitor::getUsedMemory() const {
    return total_memory - free_memory;
}

string MemoryMonitor::formatMemory(unsigned long memory_kb) const {
    //convertir de KB a GB (1 GB = 1024 * 1024 KB)
    double memory_gb = memory_kb / 1024.0 / 1024.0;

    // Formatage du nombre avec une precision de 1 decimal
    return std::format("{:.1f}", memory_gb);
}

//affichage
void MemoryMonitor::display() const {
    cout << "RAM Used: "
         << formatMemory(getUsedMemory()) << " GB / "
         << formatMemory(getTotalMemory()) << " GB"
         << endl;
}