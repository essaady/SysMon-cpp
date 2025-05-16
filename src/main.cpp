#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include<CpuMonitor.h>
#include<MemoryMonitor.h>



std::string getUptime() {
    std::ifstream fichier("/proc/uptime");
    std::string uptime_sec;
    if (fichier>> uptime_sec) {
        int seconds = std::stoi(uptime_sec);
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        std::ostringstream result;
        result << hours << "h " << minutes << "m";
        return result.str();
}
    return "Indisponible";
}

void afficherAide() {
    std::cout << "SysMon-cpp - Outil de monitoring système\n";
    std::cout << "Usage: sysmon-cpp [OPTIONS]\n\n";
    std::cout << "Options disponibles:\n";
    std::cout << "  --help                 Affiche cette aide\n";
    std::cout << "  --export-json [chemin] Exporte les données système en JSON\n";
    std::cout << "  --update-interval [s]  Change dynamiquement l’intervalle de mise à jour\n\n";
    std::cout << "Exemples:\n";
    std::cout << "  sysmon-cpp --export-json /home/user/stats.json\n";
    std::cout << "  sysmon-cpp --update-interval 5\n";
}

// Fonction pour exporter les données en JSON
void exporterJSON(const std::string& chemin) {
    std::ofstream fichier(chemin);
    if (!fichier) {
        std::cerr << "Erreur: Impossible d'écrire dans le fichier " << chemin << "\n";
        return;
}
fichier << "{\n";
fichier << "  \"cpu_usage\": \"" << CpuMonitor().getTotalUsage() << "%\",\n";
fichier << "  \"memory_usage\": \"" << MemoryMonitor().getUsedMemory() << " MB\",\n"; 
fichier << "  \"uptime\": \"" << getUptime() << "\"\n";
fichier << "}\n";
fichier.close();
std::cout << "Export JSON réussi dans: " << chemin << "\n";
}


// Fonction pour modifier dynamiquement l’intervalle de mise à jour
void mettreAJourIntervalle(int intervalle) {
    if (intervalle < 1) {
        std::cerr << "Erreur: L’intervalle doit être d’au moins 1 seconde.\n";
        return;
}
    std::cout << "Mise à jour de l'intervalle à " << intervalle << " secondes\n";
    // Ajouter la logique pour modifier dynamiquement l’intervalle sans redémarrer
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Erreur: aucun argument fourni.\n";
        afficherAide();
        return 1;
}

    std::string commande = argv[1];

    if (commande == "--help") {
        afficherAide();
} else if (commande == "--export-json") {
        if (argc < 3) {
            std::cerr << "Erreur: veuillez spécifier un chemin pour l'export.\n";
            return 1;
}
        exporterJSON(argv[2]);
} else if (commande == "--update-interval") {
        if (argc < 3) {
            std::cerr << "Erreur: veuillez spécifier un intervalle en secondes.\n";
            return 1;
}
        mettreAJourIntervalle(std::stoi(argv[2]));
} else {
        std::cerr << "Commande inconnue.\n";
        afficherAide();
}

    return 0;
}
