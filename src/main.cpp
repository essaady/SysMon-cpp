#include <iostream>
#include <unistd.h>
#include <pwd.h>
using namespace std;

int main() {
    struct passwd *utilisateur;
    utilisateur = getpwuid(geteuid());

    if (utilisateur) {
        cout << "Utilisateur connecté : " << utilisateur->pw_name << endl;
    } else {
        cout << "Erreur : utilisateur non trouvé." << endl;
    }

    return 0;
}
