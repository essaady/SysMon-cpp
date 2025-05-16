#include <iostream>
#include <iomanip>  // Pour std::setw
#include <string>


void afficherEnTete(const std::string& Titre) {
    std::cout << "\n";
    std::cout << "======================= " << Titre << " ====================\n";
}


void afficherSeparateur() {
    std::cout << "------------------------------------------------------------\n";
}

int main() {


    afficherEnTete("Processeur (CPU)");
    std::cout << std::setw(20) << "Utilisation (%)"
              << std::setw(20) << "Cœurs"
              << std::setw(20) << "Fréquence (GHz)" << "\n";
    afficherSeparateur();


    std::cout << std::setw(20) << "25.6"
              << std::setw(20) << "4"
              << std::setw(20) << "2.6" << "\n";


    afficherEnTete("  Mémoire  ");
    std::cout << std::setw(20) << "Utilisé (MB)"
              << std::setw(20) << "Total (MB)"
              << std::setw(20) << "Utilisation (%)" << "\n";
    afficherSeparateur();

    std::cout << std::setw(20) << "2048"
              << std::setw(20) << "4096"
              << std::setw(20) << "50%" << "\n";


    afficherEnTete("Processus actifs");
    std::cout << std::setw(20)  << "PID"
              << std::setw(20) << "Commande"
              << std::setw(20) << "CPU (%)" << "\n";
    afficherSeparateur();

    std::cout << std::setw(20)  << "1345"
              << std::setw(20) << "firefox"
              << std::setw(20) << "12.5" << "\n";

    std::cout << std::setw(20)  << "2768"
              << std::setw(20) << "code"
              << std::setw(20) << "8.7" << "\n";

    return 0;
}
