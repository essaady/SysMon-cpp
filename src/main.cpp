#include <iostream>
#include <string>
#include <pwd.h>
#include <unistd.h>

std::string getConnectedUser() {
    struct passwd* pw = getpwuid(geteuid());
    if (pw != nullptr) {
        return std::string(pw->pw_name); 
    } else {
        return "Utilisateur inconnu"; 
    }
}

int main() {
    std::string username = getConnectedUser();

    std::cout << " Utilisateur connecté : " << username << std::endl;

    return 0;
}
