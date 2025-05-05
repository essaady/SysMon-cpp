//error_handling_proc.cpp

#include <iostream>
#include <fstream>
#include <stdexcept>

//Fonction "getCpuCores()" pour obtenir les cœurs CPU avec une valeur par défaut en cas d'erreur.  
void readProcFile(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            throw std::runtime_error("Erreur : Impossible d'ouvrir " + filePath);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
}

int getCpuCores() {
    std::ifstream file("/proc/cpuinfo");
    if (!file.is_open()) {

/*Affichage des erreurs via "std::cerr" sans crasher le programme (tout en continuant l’exécution)*/

        std::cerr << "Erreur : Impossible de lire /proc/cpuinfo, valeur par défaut utilisée." << std::endl;
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

int main() {
    std::cout << "Lecture de /proc/cpuinfo :" << std::endl;
    readProcFile("/proc/cpuinfo");

    std::cout << "Lecture de /proc/meminfo :" << std::endl;
    readProcFile("/proc/meminfo");

    std::cout << "Nombre de cœurs CPU : " << getCpuCores() << std::endl;

    return 0;
}

  
