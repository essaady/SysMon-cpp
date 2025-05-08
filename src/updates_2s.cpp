#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

int main() {
    while (true) {
        // Mettre à jour l'affichage ici
        cout << " mises à jour..." <<endl;
        
        // Attendre 2s
        this_thread::sleep_for(chrono::seconds(2));
    }

    return 0;
}
