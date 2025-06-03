#include <iostream>
#include <sys/statvfs.h>

void afficherUtilisationDisque(const std::string& chemin = "/") {
    struct statvfs stats;

    if (statvfs(chemin.c_str(), &stats) != 0) {
        std::cerr << "Erreur lors de la récupération des informations du disque.\n";
        return;
    }

    unsigned long taille_totale = stats.f_blocks * stats.f_frsize;
    unsigned long taille_disponible = stats.f_bavail * stats.f_frsize;
    unsigned long taille_utilisee = taille_totale - taille_disponible;
    double pourcentage_utilisation = (double)taille_utilisee / taille_totale * 100;

    std::cout << "=== Utilisation du disque pour: " << chemin << " ===\n";
    std::cout << "Taille totale       : " << taille_totale / (1024 * 1024) << " MB\n";
    std::cout << "Espace utilisé      : " << taille_utilisee / (1024 * 1024) << " MB\n";
    std::cout << "Espace disponible   : " << taille_disponible / (1024 * 1024) << " MB\n";
    std::cout << "Pourcentage utilisé : " << pourcentage_utilisation << " %\n";
}
