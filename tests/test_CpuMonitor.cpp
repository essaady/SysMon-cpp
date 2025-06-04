#include "CpuMonitor.h"
#include <iostream>
#include <cassert>
#include <chrono>
#include <thread>
#include <fstream>
#include <filesystem>

using namespace std;

// Classe pour les tests unitaires
class CpuMonitorTester {
private:
    int testsRun = 0;
    int testsPassed = 0;
    int testsFailed = 0;

public:
    void runTest(const string& testName, bool condition) {
        testsRun++;
        cout << "🧪 Test: " << testName << " ... ";
        
        if (condition) {
            cout << "✅ PASSÉ" << endl;
            testsPassed++;
        } else {
            cout << "❌ ÉCHOUÉ" << endl;
            testsFailed++;
        }
    }
    
    void runTest(const string& testName, function<void()> testFunc) {
        testsRun++;
        cout << "🧪 Test: " << testName << " ... ";
        
        try {
            testFunc();
            cout << "✅ PASSÉ" << endl;
            testsPassed++;
        } catch (const exception& e) {
            cout << "❌ ÉCHOUÉ - " << e.what() << endl;
            testsFailed++;
        } catch (...) {
            cout << "❌ ÉCHOUÉ - Exception inconnue" << endl;
            testsFailed++;
        }
    }
    
    void printSummary() {
        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║              RÉSULTATS TESTS           ║" << endl;
        cout << "╠════════════════════════════════════════╣" << endl;
        cout << "║ Tests exécutés: " << setw(22) << testsRun << " ║" << endl;
        cout << "║ Tests réussis:  " << setw(22) << testsPassed << " ║" << endl;
        cout << "║ Tests échoués:  " << setw(22) << testsFailed << " ║" << endl;
        cout << "╚════════════════════════════════════════╝" << endl;
        
        if (testsFailed == 0) {
            cout << "\n🎉 Tous les tests sont passés avec succès!" << endl;
        } else {
            cout << "\n⚠️  " << testsFailed << " test(s) ont échoué." << endl;
        }
    }
};

// Tests des constructeurs
void testConstructors(CpuMonitorTester& tester) {
    cout << "\n📋 === TESTS DES CONSTRUCTEURS ===" << endl;
    
    tester.runTest("Constructeur par défaut", []() {
        CpuMonitor monitor;
        assert(monitor.getUpdateInterval() == 1);
        assert(!monitor.getMonitoringStatus());
        assert(monitor.getCurrentUsage() >= 0.0);
    });
    
    tester.runTest("Constructeur avec paramètre valide", []() {
        CpuMonitor monitor(5);
        assert(monitor.getUpdateInterval() == 5);
        assert(!monitor.getMonitoringStatus());
    });
    
    tester.runTest("Constructeur avec paramètre invalide", []() {
        CpuMonitor monitor(-1);  // Devrait être corrigé à 1
        assert(monitor.getUpdateInterval() >= 1);
    });
}

// Tests des getters et setters
void testGettersSetters(CpuMonitorTester& tester) {
    cout << "\n🔧 === TESTS GETTERS/SETTERS ===" << endl;
    
    tester.runTest("Getter/Setter intervalle valide", []() {
        CpuMonitor monitor;
        monitor.setUpdateInterval(10);
        assert(monitor.getUpdateInterval() == 10);
    });
    
    tester.runTest("Setter intervalle invalide", []() {
        CpuMonitor monitor;
        int originalInterval = monitor.getUpdateInterval();
        monitor.setUpdateInterval(-5);  // Invalide
        assert(monitor.getUpdateInterval() == originalInterval);  // Pas changé
    });
    
    tester.runTest("Test état monitoring initial", []() {
        CpuMonitor monitor;
        assert(!monitor.getMonitoringStatus());
    });
    
    tester.runTest("Test getCpuInfo non vide", []() {
        CpuMonitor monitor;
        string info = monitor.getCpuInfo();
        assert(!info.empty());
    });
}

// Tests de mesure CPU
void testCpuMeasurement(CpuMonitorTester& tester) {
    cout << "\n📊 === TESTS MESURE CPU ===" << endl;
    
    tester.runTest("Mesure CPU dans plage valide", []() {
        CpuMonitor monitor;
        monitor.updateCpuUsage();
        double usage = monitor.getCurrentUsage();
        assert(usage >= 0.0 && usage <= 100.0);
    });
    
    tester.runTest("Mesures multiples cohérentes", []() {
        CpuMonitor monitor;
        
        // Première mesure (peut être 0)
        monitor.updateCpuUsage();
        double usage1 = monitor.getCurrentUsage();
        
        // Attendre un peu et mesurer à nouveau
        this_thread::sleep_for(chrono::milliseconds(100));
        monitor.updateCpuUsage();
        double usage2 = monitor.getCurrentUsage();
        
        // Les deux mesures doivent être valides
        assert(usage1 >= 0.0 && usage1 <= 100.0);
        assert(usage2 >= 0.0 && usage2 <= 100.0);
    });
    
    tester.runTest("Format string CPU non vide", []() {
        CpuMonitor monitor;
        monitor.updateCpuUsage();
        string formatted = monitor.formatUsageString();
        assert(!formatted.empty());
        assert(formatted.find("CPU:") != string::npos);
    });
}

// Tests de monitoring
void testMonitoring(CpuMonitorTester& tester) {
    cout << "\n⚡ === TESTS MONITORING ===" << endl;
    
    tester.runTest("Démarrage/Arrêt monitoring", []() {
        CpuMonitor monitor;
        
        // Test démarrage
        monitor.startMonitoring();
        assert(monitor.getMonitoringStatus());
        
        // Attendre un peu pour s'assurer que le thread fonctionne
        this_thread::sleep_for(chrono::milliseconds(500));
        assert(monitor.getMonitoringStatus());
        
        // Test arrêt
        monitor.stopMonitoring();
        assert(!monitor.getMonitoringStatus());
    });
    
    tester.runTest("Double démarrage monitoring", []() {
        CpuMonitor monitor;
        
        monitor.startMonitoring();
        assert(monitor.getMonitoringStatus());
        
        // Deuxième démarrage (ne devrait rien faire)
        monitor.startMonitoring();
        assert(monitor.getMonitoringStatus());
        
        monitor.stopMonitoring();
        assert(!monitor.getMonitoringStatus());
    });
    
    tester.runTest("Double arrêt monitoring", []() {
        CpuMonitor monitor;
        
        // Arrêt sans démarrage
        monitor.stopMonitoring();
        assert(!monitor.getMonitoringStatus());
        
        // Démarrer puis arrêter deux fois
        monitor.startMonitoring();
        monitor.stopMonitoring();
        monitor.stopMonitoring();  // Deuxième arrêt
        assert(!monitor.getMonitoringStatus());
    });
}

// Tests d'export
void testExport(CpuMonitorTester& tester) {
    cout << "\n💾 === TESTS EXPORT ===" << endl;
    
    tester.runTest("Export vers fichier", []() {
        CpuMonitor monitor;
        monitor.updateCpuUsage();
        
        string filename = "test_export.txt";
        monitor.exportToFile(filename);
        
        // Vérifier que le fichier existe
        ifstream file(filename);
        assert(file.is_open());
        
        // Vérifier le contenu
        string line;
        bool foundReport = false;
        while (getline(file, line)) {
            if (line.find("RAPPORT CPU") != string::npos) {
                foundReport = true;
                break;
            }
        }
        file.close();
        
        // Nettoyer
        remove(filename.c_str());
        
        assert(foundReport);
    });
}

// Tests des méthodes statiques
void testStaticMethods(CpuMonitorTester& tester) {
    cout << "\n🌐 === TESTS MÉTHODES STATIQUES ===" << endl;
    
    tester.runTest("getSystemInfo non vide", []() {
        string sysInfo = CpuMonitor::getSystemInfo();
        assert(!sysInfo.empty());
        assert(sysInfo.find("Système") != string::npos);
    });
}

// Tests de performance et stress
void testPerformance(CpuMonitorTester& tester) {
    cout << "\n🚀 === TESTS PERFORMANCE ===" << endl;
    
    tester.runTest("Performance mesure CPU", []() {
        CpuMonitor monitor;
        
        auto start = chrono::high_resolution_clock::now();
        
        // Effectuer 100 mesures
        for (int i = 0; i < 100; ++i) {
            monitor.updateCpuUsage();
        }
        
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        // Les 100 mesures ne devraient pas prendre plus de 5 secondes
        assert(duration.count() < 5000);
        cout << " (" << duration.count() << "ms pour 100 mesures)";
    });
    
    tester.runTest("Monitoring de courte durée", []() {
        CpuMonitor monitor(1);  // 1 seconde d'intervalle
        
        monitor.startMonitoring();
        
        // Laisser tourner pendant 3 secondes
        this_thread::sleep_for(chrono::seconds(3));
        
        monitor.stopMonitoring();
        
        // Vérifier que le monitoring s'est bien arrêté
        assert(!monitor.getMonitoringStatus());
    });
}

// Tests de robustesse
void testRobustness(CpuMonitorTester& tester) {
    cout << "\n🛡️  === TESTS ROBUSTESSE ===" << endl;
    
    tester.runTest("Export fichier invalide", []() {
        CpuMonitor monitor;
        // Tenter d'exporter vers un chemin invalide
        monitor.exportToFile("/root/invalid_path_test.txt");
        // Le test passe s'il n'y a pas de crash
        assert(true);
    });
    
    tester.runTest("Changements multiples d'intervalle", []() {
        CpuMonitor monitor;
        
        // Changer l'intervalle plusieurs fois rapidement
        for (int i = 1; i <= 10; ++i) {
            monitor.setUpdateInterval(i);
            assert(monitor.getUpdateInterval() == i);
        }
    });
    
    tester.runTest("Destruction avec monitoring actif", []() {
        {
            CpuMonitor monitor;
            monitor.startMonitoring();
            this_thread::sleep_for(chrono::milliseconds(100));
            // Le destructeur devrait nettoyer proprement
        }
        // Si on arrive ici, le destructeur a fonctionné
        assert(true);
    });
}

int main() {
    cout << "╔════════════════════════════════════════════════╗" << endl;
    cout << "║            TESTS UNITAIRES CPU MONITOR         ║" << endl;
    cout << "║               Version Améliorée                ║" << endl;
    cout << "╚════════════════════════════════════════════════╝" << endl;

    CpuMonitorTester tester;
    
    try {
        // Exécuter tous les tests
        testConstructors(tester);
        testGettersSetters(tester);
        testCpuMeasurement(tester);
        testMonitoring(tester);
        testExport(tester);
        testStaticMethods(tester);
        testPerformance(tester);
        testRobustness(tester);
        
    } catch (const exception& e) {
        cout << "\n💥 Erreur critique durant les tests: " << e.what() << endl;
        return 1;
    }
    
    // Afficher le résumé
    tester.printSummary();
    
    return 0;
    