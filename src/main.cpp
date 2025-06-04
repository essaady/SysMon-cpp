#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

int main() {
    uid_t uid = geteuid();  
    struct passwd *pw = getpwuid(uid); 

    if (pw) {
        printf("Utilisateur connecté : %s\n", pw->pw_name);
    } else {
        perror("Erreur lors de la récupération du nom d'utilisateur");
        return 1;
    }

    return 0;
}