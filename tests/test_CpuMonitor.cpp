#include "../include/CpuMonitor.h"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

// Fonction simple pour tester l'égalité des doubles avec une tolérance
bool doubleEquals(double a, double b, double epsilon = 0.001) {
    return std::abs(a - b) < epsilon;
}

void testCpuMonitorInitialization() {
    std::cout << "Testing CpuMonitor initialization..." << std::endl;
    
    CpuMonitor monitor;
    
    // Vérifier que l'utilisation totale est dans une plage raisonnable
    double usage = monitor.getTotalUsage();
    assert(usage >= 0.0 && usage <= 100.0);
    
    std::cout << "CpuMonitor initialization test passed." << std::endl;
}

void testCpuMonitorUpdate() {
    std::cout << "Testing CpuMonitor update..." << std::endl;
    
    CpuMonitor monitor;
    
    // Première mise à jour
    bool updateResult = monitor.update();
    assert(updateResult);
    
    // Attendre un peu pour avoir des données significatives
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Deuxième mise à jour
    updateResult = monitor.update();
    assert(updateResult);
    
    // Vérifier que l'utilisation totale est dans une plage raisonnable
    double usage = monitor.getTotalUsage();
    assert(usage >= 0.0 && usage <= 100.0);
    
    std::cout << "CpuMonitor update test passed." << std::endl;
}

void testCpuMonitorCoreUsage() {
    std::cout << "Testing CpuMonitor core usage..." << std::endl;
    
    CpuMonitor monitor;
    
    // Mettre à jour pour avoir des données
    monitor.update();
    
    // Attendre un peu pour avoir des données significatives
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Mettre à jour à nouveau
    monitor.update();
    
    // Obtenir l'utilisation par cœur
    auto coreUsage = monitor.getCoreUsage();
    
    // Vérifier qu'il y a au moins un cœur
    assert(!coreUsage.empty());
    
    // Vérifier que chaque utilisation est dans une plage raisonnable
    for (const auto& pair : coreUsage) {
        assert(pair.second >= 0.0 && pair.second <= 100.0);
    }
    
    std::cout << "CpuMonitor core usage test passed." << std::endl;
}

void testCpuMonitorExport() {
    std::cout << "Testing CpuMonitor export..." << std::endl;
    
    CpuMonitor monitor;
    
    // Mettre à jour pour avoir des données
    monitor.update();
    
    // Exporter au format texte
    std::string textExport = monitor.exportAsText();
    assert(!textExport.empty());
    assert(textExport.find("CPU Usage") != std::string::npos);
    
    // Exporter au format CSV
    std::string csvExport = monitor.exportAsCSV();
    assert(!csvExport.empty());
    assert(csvExport.find("CPU,Usage") != std::string::npos);
    
    std::cout << "CpuMonitor export test passed." << std::endl;
}

int main() {
    std::cout << "Running CpuMonitor tests..." << std::endl;
    
    testCpuMonitorInitialization();
    testCpuMonitorUpdate();
    testCpuMonitorCoreUsage();
    testCpuMonitorExport();
    
    std::cout << "All CpuMonitor tests passed!" << std::endl;
    
    return 0;
}
