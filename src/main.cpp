#include <iostream>
#include <sys/statvfs.h>   // Pour statvfs()
#include <unistd.h>        // Pour system("clear")
#include <thread>          // Pour std::this_thread::sleep_for
#include <chrono>          // Pour std::chrono::seconds
#include <iomanip>         // Pour std::setprecision

// Fonction qui affiche l'utilisation du disque sur le chemin donné
void afficherUtilisationDisque(const char* chemin) {
    struct statvfs stat;

    // Récupérer les informations du système de fichiers
    if (statvfs(chemin, &stat) != 0) {
        std::cerr << "Erreur: impossible de récupérer les infos du disque pour " << chemin << std::endl;
        return;
    }

    // Calcul des tailles en octets
    unsigned long long tailleTotale = stat.f_blocks * stat.f_frsize;
    unsigned long long espaceDisponible = stat.f_bavail * stat.f_frsize;
    unsigned long long espaceUtilise = tailleTotale - espaceDisponible;

    // Calcul du pourcentage d'utilisation
    double pourcentageUtilisation = 0.0;
    if (tailleTotale > 0) {
        pourcentageUtilisation = (double)espaceUtilise / tailleTotale * 100.0;
    }

    // Affichage avec 2 chiffres après la virgule
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Chemin : " << chemin << std::endl;
    std::cout << "Taille totale     : " << tailleTotale / (1024.0 * 1024 * 1024) << " Go" << std::endl;
    std::cout << "Espace utilisé    : " << espaceUtilise / (1024.0 * 1024 * 1024) << " Go" << std::endl;
    std::cout << "Espace disponible : " << espaceDisponible / (1024.0 * 1024 * 1024) << " Go" << std::endl;
    std::cout << "Pourcentage utilisé : " << pourcentageUtilisation << " %" << std::endl;
}

int main() {
    const char* chemin = "/";  // Partition à surveiller, ici la racine

    while (true) {
        system("clear");         // Effacer la console pour un affichage propre
        afficherUtilisationDisque(chemin);
        std::this_thread::sleep_for(std::chrono::seconds(1));  // Pause d'1 seconde
    }

    return 0;
}
