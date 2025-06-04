#include "CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <unistd.h>   // Pour sysconf
#include <iostream>

CpuMonitor::CpuMonitor() {
    nombreCpus = sysconf(_SC_NPROCESSORS_ONLN); // Nombre total de CPU disponibles
    listeCpus.resize(nombreCpus); // Redimensionne le vecteur
}

bool CpuMonitor::mettreAJour() {
    std::ifstream fichier("/proc/stat");
    if (!fichier.is_open()) return false;

    std::string ligne;
    int indice = 0;

    while (std::getline(fichier, ligne)) {
        if (ligne.substr(0, 3) == "cpu" && ligne[3] != ' ') {
            std::istringstream flux(ligne);
            std::string etiquette;
            long utilisateur, gentil, systeme, inactif, attenteIO, interruption, interruptionLogicielle, vole;

            flux >> etiquette >> utilisateur >> gentil >> systeme >> inactif >> attenteIO >> interruption >> interruptionLogicielle >> vole;

            long tempsInactif = inactif + attenteIO;
            long tempsActif = utilisateur + gentil + systeme + interruption + interruptionLogicielle + vole;
            long tempsTotal = tempsInactif + tempsActif;

            float utilisation = (float)(tempsActif) / tempsTotal * 100;
            listeCpus[indice].utilisation = utilisation;
            indice++;

            if (indice >= nombreCpus) break;
        }
    }

    return true;
}

float CpuMonitor::obtenirUtilisationMoyenne() {
    float total = 0.0;
    for (const auto& cpu : listeCpus) {
        total += cpu.utilisation;
    }

    if (nombreCpus == 0) return 0;
    return total / nombreCpus;
}

float CpuMonitor::obtenirFrequenceMoyenne() {
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string ligne;
    float sommeFrequences = 0.0;
    int compteur = 0;

    while (std::getline(cpuinfo, ligne)) {
        if (ligne.find("cpu MHz") != std::string::npos) {
            size_t position = ligne.find(":");
            if (position != std::string::npos) {
                float mhz = std::stof(ligne.substr(position + 1));
                sommeFrequences += mhz;
                compteur++;
            }
        }
    }

    if (compteur == 0) return 0;
    return sommeFrequences / compteur;
}

std::string CpuMonitor::obtenirInfosCpu() {
    std::string infos = "Nombre de CPU : " + std::to_string(nombreCpus) + "\n";
    infos += "Utilisation moyenne : " + std::to_string(obtenirUtilisationMoyenne()) + " %\n";
    infos += "Fréquence moyenne : " + std::to_string(obtenirFrequenceMoyenne()) + " MHz\n";
    return infos;
}

int CpuMonitor::getNombreCpus() const {
    return nombreCpus;
}

std::vector<Processeur> CpuMonitor::getListeCpus() const {
    return listeCpus;
}
