#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <signal.h>
#include <getopt.h>
#include "SysMon.h"

using namespace std;

bool running = true;

void signalHandler(int signum) {
    cout << "\nArrêt du monitoring..." << endl;
    running = false;
}

void printHelp() {
    cout << "=== SYSTÈME DE SURVEILLANCE - AIDE ===" << endl;
    cout << "Usage: ./sysmon [OPTIONS]" << endl << endl;
    cout << "Options:" << endl;
    cout << "  -h, --help              Afficher cette aide" << endl;
    cout << "  -i, --interval SECONDS  Intervalle de mise à jour (défaut: 1 seconde)" << endl;
    cout << "  -e, --export FORMAT     Format d'export (text|csv)" << endl;
    cout << "  -o, --output FILE       Fichier de sortie pour l'export" << endl;
    cout << "  -c, --continuous        Mode surveillance continue" << endl;
    cout << "  -s, --single            Une seule lecture puis arrêt" << endl << endl;
    cout << "Exemples:" << endl;
    cout << "  ./sysmon                           # Affichage unique" << endl;
    cout << "  ./sysmon -c -i 5                   # Surveillance continue toutes les 5s" << endl;
    cout << "  ./sysmon -e csv -o monitoring.csv  # Export CSV dans un fichier" << endl;
    cout << "  ./sysmon -s -e text                # Une lecture, export texte" << endl;
}

int main(int argc, char* argv[]) {
    int interval = 1;
    string exportFormat = "text";
    string outputFile = "";
    bool continuous = false;
    bool single = true;
    
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"interval", required_argument, 0, 'i'},
        {"export", required_argument, 0, 'e'},
        {"output", required_argument, 0, 'o'},
        {"continuous", no_argument, 0, 'c'},
        {"single", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };
    
    int c;
    int option_index = 0;
    
    while ((c = getopt_long(argc, argv, "hi:e:o:cs", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                help = true;
                printHelp();
                return 0;
            case 'i':
                interval = atoi(optarg);
                if (interval <= 0) {
                    cerr << "Erreur: L'intervalle doit être positif" << endl;
                    return 1;
                }
                updateInterval = interval * 1000; 
                break;
            case 'e':
                exportFormat = optarg;
                if (exportFormat != "text" && exportFormat != "csv") {
                    cerr << "Erreur: Format d'export invalide. Utilisez 'text' ou 'csv'" << endl;
                    return 1;
                }
                break;
            case 'o':
                outputFile = optarg;
                exportPath = outputFile;
                break;
            case 'c':
                continuous = true;
                single = false;
                break;
            case 's':
                single = true;
                continuous = false;
                break;
            case '?':
                cerr << "Option inconnue. Utilisez -h pour l'aide." << endl;
                return 1;
            default:
                break;
        }
    }
    
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    
    cout << "=== DÉMARRAGE DU SYSTÈME DE SURVEILLANCE ===" << endl;
    cout << getInfo() << endl;
    
    if (continuous) {
        cout << "Mode surveillance continue activé (Ctrl+C pour arrêter)" << endl;
        cout << "Intervalle: " << interval << " seconde(s)" << endl;
        if (!outputFile.empty()) {
            cout << "Export vers: " << outputFile << " (format: " << exportFormat << ")" << endl;
        }
        cout << "----------------------------------------" << endl;
    }
    
    do {
        if (update(cpuMonitor, memoryMonitor, processMonitor)) {
            string output;
            
            if (exportFormat == "csv") {
                output = exportAsCSV();
            } else {
                output = exportAsText();
            }
            
            if (outputFile.empty()) {
                cout << output << endl;
                if (continuous) {
                    cout << "----------------------------------------" << endl;
                }
            } else {
                
                ofstream outFile;
                if (continuous) {
                    outFile.open(outputFile, ios::app); 
                } else {
                    outFile.open(outputFile);
                }
                
                if (outFile.is_open()) {
                    outFile << output << endl;
                    outFile.close();
                    if (!continuous) {
                        cout << "Données exportées vers: " << outputFile << endl;
                    }
                } else {
                    cerr << "Erreur: Impossible d'écrire dans le fichier " << outputFile << endl;
                    return 1;
                }
            }
        } else {
            cerr << "Erreur lors de la mise à jour des données de surveillance" << endl;
            return 1;
        }
        
        if (continuous && running) {
            this_thread::sleep_for(chrono::seconds(interval));
        }
        
    } while (continuous && running);
    
    if (!running) {
        cout << "Surveillance arrêtée par l'utilisateur." << endl;
    }
    
    cout << "=== FIN DE LA SURVEILLANCE ===" << endl;
    return 0;
}