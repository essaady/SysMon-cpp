#include "../include/CpuMonitor.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <thread> 

using namespace std;

// structure pour stocker les temps CPU
struct CpuTime {
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
    long steal;
    long guest;
    long guest_nice;
};

// constructeur
CpuMonitor::CpuMonitor() {
    cout << "Initialisation du moniteur CPU..." << endl;
}

// destructeur
CpuMonitor::~CpuMonitor() {
    cout << "Arrêt du moniteur CPU..." << endl;
}

// fct pour lire les temps CPU à partir de /proc/stat
bool CpuMonitor::readCpuStats(vector<CpuTime>& cpuTimes) {
    ifstream statFile("/proc/stat");
    
    if (!statFile.is_open()) {
        cerr << "Erreur: Impossible d'ouvrir /proc/stat" << endl;
        return false;
    }
    
    string line;
    cpuTimes.clear();
    
    while (getline(statFile, line)) {
        if (line.find("cpu") == 0) {
            istringstream iss(line);
            string cpu;
            CpuTime ctime = {0};
            
            iss >> cpu;
            
       // ignorer la ligne "cpu" qui représente le total
            if (cpu != "cpu") {
                iss >> ctime.user >> ctime.nice >> ctime.system >> ctime.idle
                    >> ctime.iowait >> ctime.irq >> ctime.softirq
                    >> ctime.steal >> ctime.guest >> ctime.guest_nice;
                
                cpuTimes.push_back(ctime);
            }
        }
    }
    
    statFile.close();
    return true;
}

// fct pour calculer le pourcentage d'utilisation CPU
double CpuMonitor::calculateCpuUsage(const CpuTime& prev, const CpuTime& curr) {
    long prevIdle = prev.idle + prev.iowait;
    long currIdle = curr.idle + curr.iowait;
    
    long prevNonIdle = prev.user + prev.nice + prev.system + prev.irq + prev.softirq + prev.steal;
    long currNonIdle = curr.user + curr.nice + curr.system + curr.irq + curr.softirq + curr.steal;
    
    long prevTotal = prevIdle + prevNonIdle;
    long currTotal = currIdle + currNonIdle;
    
    long totalDelta = currTotal - prevTotal;
    long idleDelta = currIdle - prevIdle;
    
// éviter la division par zéro
    if (totalDelta == 0) {
        return 0.0;
    }
    
    return 100.0 * (totalDelta - idleDelta) / totalDelta;
}

// fct principale pour monitorer l'utilisation CPU
void CpuMonitor::start() {
    vector<CpuTime> prevCpuTimes;
    vector<CpuTime> currCpuTimes;
    
    cout << "Démarrage du moniteur CPU..." << endl;
    cout << "Appuyez sur Ctrl+C pour quitter." << endl;
    
// boucle infini pour mettre à jour les informations touts les 2s
    while (true) {
// première lectur
        if (!readCpuStats(prevCpuTimes)) {
            cerr << "Erreur lors de la lecture des statistiques CPU." << endl;
            return;
        }
        
// attendre 2s
    this_thread::sleep_for(chrono::seconds(2));
        
   // deuxième lecture
        if (!readCpuStats(currCpuTimes)) {
            cerr << "Erreur lors de la lecture des statistiques CPU." << endl;
            return;
        }
        
        // afficher l'utilisation pour chaque coeur
        cout << "\033[2J\033[1;1H";  // effacer l'écran et positionner le curseur en haut à gauche
        cout << "===== Utilisation CPU =====\n" << endl;
        
        for (size_t i = 0; i < currCpuTimes.size() && i < prevCpuTimes.size(); ++i) {
            double usage = calculateCpuUsage(prevCpuTimes[i], currCpuTimes[i]);
            cout << "CPU" << i << ": " << usage << "%" << endl;
            
// afficher une barr de progression simple
            cout << "[";
            int barWidth = 50;
            int pos = barWidth * usage / 100.0;
            
            for (int j = 0; j < barWidth; ++j) {
                if (j < pos) cout << "=";
                else if (j == pos) cout << ">";
                else cout << " ";
            }
            
            cout << "] " << endl << endl;
        }
        
        cout << "Mise à jour toutes les 2 secondes..." << endl;
    }
}

// si vous voulez teste le module indépendamment
#ifdef TEST_CPU_MONITOR
int main() {
    CpuMonitor monitor;
    monitor.start();
    return 0;
}
#endif