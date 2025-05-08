#include <iostream>
#include <thread> //pour utiliser std::this_thread::sleep_for() (mise en pause du programme)
#include "../include/MemoryMonitor.h"

using namespace std;

int main() {

    MemoryMonitor memMonitor;

    cout << "SysMon-cpp - Moniteur de memoire" << endl;
    cout << "--------------------------------" << endl;

    //boucle principale pour mettre a jour et afficher les informations
    //affiche les informations toutes les 2 secondes indefiniment
    while (true) {
        //mettre a jour les donnees de memoire
        if (memMonitor.update()) {
            //afficher l'heure actuelle
            time_t now = time(0);
            char* dt = ctime(&now);
            cout << "Mise a jour: " << dt;

            //afficher les informations de memoire
            memMonitor.display();
        } else {
            cout << "Erreur lors de la mise a jour des informations de memoire." << endl;
        }

        //attendre 2 secondes avant la prochaine mise a jour
        std::this_thread::sleep_for(2000); //temps d'attente en millisecondes (2000 ms = 2 secondes)
    }

    return 0;
}