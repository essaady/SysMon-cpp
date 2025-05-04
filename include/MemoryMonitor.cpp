#include<iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

float get_used_memory() {
    ifstream meminfo("/proc/meminfo");
    if (!meminfo) {
        cout << "Impossible " << endl; 
        return 0.0;
    }

    string line;
    long mem_total = 0, mem_free = 0, buffers = 0, cached = 0;

    while (getline(meminfo, line)) {
        stringstream mo(line);
        string k;
        long valeur;
        string unite;

        // lire chaque ligne et extraire les données
        mo >> k >> valeur >> unite;

        if (k == "MemTotal:") {
            mem_total = valeur;
        } else if (k == "MemFree:") {
            mem_free = valeur;
        } else if (k == "Buffers:") {
            buffers = valeur;
        } else if (k == "Cached:") {
            cached = valeur;
        }

        //  pour arrêter la lecture
        if (mem_total && mem_free && buffers && cached) {
            break;
        }
    }

    // calculer la mémoire en Mo
    long used_memory = mem_total - mem_free - buffers - cached;
    return static_cast<float>(used_memory) / 1024; // Conversion en Mo
}

int main() {
    float memoire_ut = get_used_memory();
    cout << "Mémoire utilisée : " << memoire_ut << " MB" << endl;
    return 0;
}