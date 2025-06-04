#include "CpuMonitor.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <limits>

using namespace std;

// Variable globale pour contrôler l'arrêt du programme
volatile bool keepRunning = true;

// Gestionnaire de signal pour arrêt propre (Ctrl+C)
void signalHandler(int signal) {
    cout << "\n\nArrêt du programme demandé..." << endl;
    keepRunning = false;
}

// Fonction pour effacer l'écran de manière portable
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Fonction pour afficher le menu avec style
void displayMenu() {
    cout << "\n╔═══════════════════════════════╗" << endl;
    cout << "║       CPU MONITOR MENU        ║" << endl;
    cout << "╠═══════════════════════════════╣" << endl;
    cout << "║ 1. Surveillance temps réel    ║" << endl;
    cout << "║ 2. Informations CPU           ║" << endl;
    cout << "║ 3. Mesure unique              ║" << endl;
    cout << "║ 4. Changer intervalle         ║" << endl;
    cout << "║ 5. Exporter données           ║" << endl;
    cout << "║ 6. Historique utilisation     ║" << endl;
    cout << "║ 7. Informations système       ║" << endl;
    cout << "║ 8. Test de charge             ║" << endl;
    cout << "║ 9. Démonstration POO          ║" << endl;
    cout << "║ 0. Quitter                    ║" << endl;
    cout << "╚═══════════════════════════════╝" << endl;
    cout << "Choix: ";
}

// Fonction de surveillance en temps réel améliorée
void realTimeMonitoring(CpuMonitor& monitor) {
    cout << "\nDémarrage de la surveillance temps réel..." << endl;
    cout << "Appuyez sur Ctrl+C pour arrêter\n" << endl;
    
    monitor.startMonitoring();
    
    // Attendre un peu pour que le monitoring se stabilise
    this_thread::sleep_for(chrono::milliseconds(500));

    int refreshCount = 0;
    auto startTime = chrono::steady_clock::now();

    while (keepRunning && monitor.getMonitoringStatus()) {
        clearScreen();
        
        cout << "╔════════════════════════════════════════════════╗" << endl;
        cout << "║              CPU MONITOR - TEMPS RÉEL          ║" << endl;
        cout << "╚════════════════════════════════════════════════╝" << endl;
        cout << endl;

        // Afficher l'utilisation formatée
        cout << monitor.formatUsageString() << endl;
        cout << endl;

        // Afficher timestamp et informations additionnelles
        auto now = chrono::system_clock::now();
        time_t time_now = chrono::system_clock::to_time_t(now);
        cout << "⏰ Dernière mise à jour: " << ctime(&time_now);
        
        // Afficher durée de surveillance
        auto elapsed = chrono::steady_clock::now() - startTime;
        auto seconds = chrono::duration_cast<chrono::seconds>(elapsed).count();
        cout << "📊 Durée surveillance: " << seconds << "s" << endl;
        cout << "🔄 Rafraîchissements: " << ++refreshCount << endl;
        cout << "⚙️  Intervalle: " << monitor.getUpdateInterval() << "s" << endl;
        
        cout << "\n" << string(50, '─') << endl;
        cout << "Appuyez sur Ctrl+C pour arrêter la surveillance" << endl;

        this_thread::sleep_for(chrono::seconds(monitor.getUpdateInterval()));
    }

    monitor.stopMonitoring();
    cout << "\nSurveillance terminée." << endl;
}

// Test de charge CPU pour démonstration
void stressTest(int duration) {
    cout << "\n🔥 Test de charge CPU pendant " << duration << " secondes..." << endl;
    cout << "Ceci va augmenter artificiellement l'utilisation CPU\n" << endl;
    
    auto endTime = chrono::steady_clock::now() + chrono::seconds(duration);
    vector<thread> stressThreads;
    
    // Créer des threads pour charger le CPU
    unsigned int numThreads = thread::hardware_concurrency();
    for (unsigned int i = 0; i < numThreads; ++i) {
        stressThreads.emplace_back([endTime]() {
            volatile long long counter = 0;
            while (chrono::steady_clock::now() < endTime) {
                ++counter; // Opération simple pour charger le CPU
            }
        });
    }
    
    // Attendre que tous les threads terminent
    for (auto& t : stressThreads) {
        t.join();
    }
    
    cout << "✅ Test de charge terminé!" << endl;
}

// Démonstration des concepts POO améliorée
void demonstrateOOP() {
    cout << "\n╔═══════════════════════════════════════════════╗" << endl;
    cout << "║          DÉMONSTRATION CONCEPTS POO           ║" << endl;
    cout << "╚═══════════════════════════════════════════════╝" << endl;

    cout << "\n1️⃣  CONSTRUCTEURS:" << endl;
    cout << "   • Création de deux objets CpuMonitor" << endl;
    CpuMonitor monitor1;              // Constructeur par défaut
    CpuMonitor monitor2(3);           // Constructeur avec paramètre

    cout << "\n2️⃣  ENCAPSULATION:" << endl;
    cout << "   • Accès aux données via getters/setters" << endl;
    cout << "   • Intervalle monitor1: " << monitor1.getUpdateInterval() << "s" << endl;
    cout << "   • Intervalle monitor2: " << monitor2.getUpdateInterval() << "s" << endl;

    monitor1.setUpdateInterval(5);
    cout << "   • Nouvel intervalle monitor1: " << monitor1.getUpdateInterval() << "s" << endl;

    cout << "\n3️⃣  ABSTRACTION:" << endl;
    cout << "   • Utilisation de méthodes publiques simples" << endl;
    cout << "   • Les détails d'implémentation sont cachés" << endl;
    
    monitor1.updateCpuUsage();
    cout << "   • Utilisation CPU mesurée: " << monitor1.getCurrentUsage() << "%" << endl;
    cout << "   • Affichage formaté: " << monitor1.formatUsageString() << endl;

    cout << "\n4️⃣  MÉTHODES STATIQUES:" << endl;
    cout << "   • Accès sans instanciation d'objet" << endl;
    cout << CpuMonitor::getSystemInfo() << endl;

    cout << "5️⃣  THREAD SAFETY:" << endl;
    cout << "   • Utilisation d'atomic<> et mutex" << endl;
    cout << "   • Gestion sécurisée des threads" << endl;
    
    cout << "\n✨ Démonstration POO terminée!" << endl;
}

// Fonction pour valider l'entrée utilisateur
int getValidInput(int min, int max) {
    int choice;
    while (true) {
        if (cin >> choice) {
            if (choice >= min && choice <= max) {
                return choice;
            } else {
                cout << "❌ Choix invalide! Veuillez choisir entre " << min << " et " << max << ": ";
            }
        } else {
            cout << "❌ Entrée invalide! Veuillez saisir un nombre: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Fonction pour obtenir un nom de fichier valide
string getValidFilename() {
    string filename;
    cout << "📁 Nom du fichier (avec extension): ";
    cin >> filename;
    
    // Ajouter extension par défaut si absente
    if (filename.find('.') == string::npos) {
        filename += ".txt";
        cout << "📝 Extension .txt ajoutée automatiquement" << endl;
    }
    
    return filename;
}

int main() {
    // Configurer le gestionnaire de signal
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    cout << "╔════════════════════════════════════════════════╗" << endl;
    cout << "║            BIENVENUE DANS CPU MONITOR          ║" << endl;
    cout << "║        Projet C++ - Programmation POO         ║" << endl;
    cout << "╚════════════════════════════════════════════════╝" << endl;

    // Créer l'objet principal avec intervalle par défaut
    CpuMonitor cpuMonitor(2);
    
    // Faire une première mesure pour initialiser
    cpuMonitor.updateCpuUsage();

    int choice;
    string filename;
    int interval;

    do {
        displayMenu();
        choice = getValidInput(0, 9);

        switch (choice) {
            case 1:
                realTimeMonitoring(cpuMonitor);
                keepRunning = true;  // Réinitialiser pour le menu
                break;

            case 2:
                cpuMonitor.displayCpuInfo();
                break;

            case 3:
                cout << "\n🔍 Mesure unique en cours..." << endl;
                cpuMonitor.updateCpuUsage();
                cout << cpuMonitor.formatUsageString() << endl;
                cout << "💡 Utilisation actuelle: " << cpuMonitor.getCurrentUsage() << "%" << endl;
                break;

            case 4:
                cout << "\n⚙️  Nouvel intervalle (1-60 secondes): ";
                interval = getValidInput(1, 60);
                cpuMonitor.setUpdateInterval(interval);
                break;

            case 5:
                filename = getValidFilename();
                cpuMonitor.updateCpuUsage();  // Mise à jour avant export
                cpuMonitor.exportToFile(filename);
                break;

            case 6:
                cpuMonitor.printUsageHistory();
                break;

            case 7:
                cout << "\n" << CpuMonitor::getSystemInfo() << endl;
                break;

            case 8: {
                cout << "\n🚀 Durée du test de charge (1-10 secondes): ";
                int duration = getValidInput(1, 10);
                
                cout << "\n⚠️  ATTENTION: Ce test va charger votre CPU!" << endl;
                cout << "Voulez-vous continuer? (1=Oui, 0=Non): ";
                int confirm = getValidInput(0, 1);
                
                if (confirm == 1) {
                    stressTest(duration);
                } else {
                    cout << "Test annulé." << endl;
                }
                break;
            }

            case 9:
                demonstrateOOP();
                break;

            case 0:
                cout << "\n👋 Merci d'avoir utilisé CPU Monitor!" << endl;
                cout << "Au revoir! 🚀" << endl;
                break;

            default:
                cout << "❌ Choix invalide!" << endl;
        }
        
        // Pause avant retour au menu (sauf pour surveillance temps réel et quitter)
        if (choice != 0 && choice != 1) {
            cout << "\n📱 Appuyez sur Entrée pour continuer...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 0);

    return 0;
}