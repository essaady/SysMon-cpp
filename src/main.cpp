//<<<<<<< Sofyanehab
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
// =======
#include "../include/SysMon.h"


int main(int argc, char *argv[])
{

    // clear the screen on start
    system("clear");
    vector<string> options = {"--help", "--update", "--export"};
    // checking for user argument
    int option = 0;
    if (argc >= 2)
    {
        bool isReconized = false;
        for (auto param : options)
        {
            char *argument = new char[param.length() + 1];
            strcpy(argument, param.c_str());
            if (!strcmp(argv[1], argument))
            {
                std::cout << param << " Called\n";
                isReconized = true;
                break;
            }
        }
        if (!isReconized)
        {
            std::cout << "'" << argv[1] << "' command not reconigzed\n";
            exit(-1);
        }

        if (!strcmp(argv[1], "--export"))
        {
            option = options::_NLOG;
        }
    }
    
    int updateInterval = 5e5;
    
    SysMon SysMonCpp(updateInterval);

    return SysMonCpp.run();
}
// >>>>>>> dev
