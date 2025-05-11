#include "CpuMonitor.h"
//
//
//.....
//Fonction "getCpuCores()" pour obtenir les c?urs CPU avec une valeur par d?faut en cas d'erreur.  
int getCpuCores() {
    std::ifstream file("/proc/cpuinfo");
    if (!file.is_open()) {

/*Affichage des erreurs via "std::cerr" sans crasher le programme (tout en continuant l?ex?cution)*/

        std::cerr << "Erreur : Impossible de lire /proc/cpuinfo, valeur par d?faut utilis?e." << std::endl;
        return -1;
/*Si "/proc/cpuinfo" est inaccessible, on retourne "-1" au lieu de faire crasher le programme*/
    }

    int cores = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("processor") != std::string::npos) {
            cores++;
        }
    }
    return cores;
}
