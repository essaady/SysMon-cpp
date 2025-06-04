#include "CpuMonitor.h"
#include <iomanip>
#include <unistd.h>
#include <ctime>
#include <algorithm>

using namespace std;

// Constructeur par défaut
CpuMonitor::CpuMonitor()
    : currentUsage(0.0), previousIdle(0), previousTotal(0),
      isMonitoring(false), updateInterval(1) {
    cout << "CpuMonitor créé avec intervalle par défaut (1s)" << endl;
    parseCpuInfo();
}

// Constructeur avec paramètre
CpuMonitor::CpuMonitor(int interval)
    : currentUsage(0.0), previousIdle(0), previousTotal(0),
      isMonitoring(false), updateInterval(max(1, interval)) {  // min 1 seconde
    cout << "CpuMonitor créé avec intervalle: " << updateInterval.load() << "s" << endl;
    parseCpuInfo();
}

// Destructeur amélioré
CpuMonitor::~CpuMonitor() {
    stopMonitoring();  // Ceci gère déjà le join()
    cout << "CpuMonitor détruit" << endl;
}

// Lire les statistiques CPU depuis /proc/stat
vector<long> CpuMonitor::readCpuStats() {
    vector<long> stats;
    ifstream file("/proc/stat");
    
    if (!file.is_open()) {
        cerr << "Erreur: Impossible d'ouvrir /proc/stat" << endl;
        return stats;
    }
    
    string line;
    if (getline(file, line)) {
        istringstream iss(line);
        string cpu;
        iss >> cpu;  // ignorer "cpu"
        
        long value;
        while (iss >> value) {
            stats.push_back(value);
        }
    }
    
    file.close();
    return stats;
}

// Calcul amélioré de l'utilisation du CPU
double CpuMonitor::calculateUsage(const vector<long>& stats) {
    if (stats.size() < 4) {
        cerr << "Données CPU insuffisantes" << endl;
        return 0.0;
    }

    long user = stats[0];
    long nice = stats[1];
    long system = stats[2];
    long idle = stats[3];
    long iowait = stats.size() > 4 ? stats[4] : 0;
    long irq = stats.size() > 5 ? stats[5] : 0;
    long softirq = stats.size() > 6 ? stats[6] : 0;

    long currentIdle = idle + iowait;
    long currentTotal = user + nice + system + idle + iowait + irq + softirq;

    // Première mesure : initialiser les valeurs
    if (previousTotal == 0) {
        previousTotal = currentTotal;
        previousIdle = currentIdle;
        return 0.0;  // Retourner 0 pour la première mesure
    }

    long totalDiff = currentTotal - previousTotal;
    long idleDiff = currentIdle - previousIdle;

    // Sauvegarder pour la prochaine mesure
    previousTotal = currentTotal;
    previousIdle = currentIdle;

    if (totalDiff <= 0) {
        cerr << "Avertissement: Différence de temps CPU invalide" << endl;
        return 0.0;
    }

    double usage = ((double)(totalDiff - idleDiff) / totalDiff) * 100.0;
    
    // Limiter entre 0 et 100%
    return max(0.0, min(100.0, usage));
}

// Parser les infos CPU depuis /proc/cpuinfo
void CpuMonitor::parseCpuInfo() {
    ifstream file("/proc/cpuinfo");
    ostringstream info;

    if (!file.is_open()) {
        cpuInfo = "Informations CPU non disponibles";
        return;
    }

    string line;
    bool foundModel = false;
    int coreCount = 0;

    while (getline(file, line)) {
        if (line.find("model name") != string::npos && !foundModel) {
            info << line << endl;
            foundModel = true;
        }
        if (line.find("processor") != string::npos) {
            coreCount++;
        }
    }
    
    info << "Nombre de processeurs: " << coreCount << endl;
    file.close();
    cpuInfo = info.str();
}

// Démarrer le monitoring avec vérifications
void CpuMonitor::startMonitoring() {
    if (isMonitoring.load()) {
        cout << "Monitoring déjà en cours..." << endl;
        return;
    }

    // Réinitialiser les valeurs pour un nouveau monitoring
    previousTotal = 0;
    previousIdle = 0;
    
    isMonitoring = true;
    cout << "Démarrage du monitoring CPU..." << endl;
    
    try {
        monitoringThread = thread(&CpuMonitor::monitoringLoop, this);
    } catch (const exception& e) {
        cerr << "Erreur lors du démarrage du thread: " << e.what() << endl;
        isMonitoring = false;
    }
}

// Arrêter le monitoring avec gestion propre
void CpuMonitor::stopMonitoring() {
    if (!isMonitoring.load()) {
        return;  // Pas de message si déjà arrêté
    }

    cout << "Arrêt du monitoring..." << endl;
    isMonitoring = false;
    
    if (monitoringThread.joinable()) {
        monitoringThread.join();
    }
    
    cout << "Monitoring arrêté." << endl;
}

// Boucle de monitoring améliorée
void CpuMonitor::monitoringLoop() {
    while (isMonitoring.load()) {
        updateCpuUsage();
        
        // Ajouter à l'historique de manière thread-safe
        {
            lock_guard<mutex> lock(historyMutex);
            usageHistory.push_back(currentUsage.load());
            
            // Limiter la taille de l'historique (garder les 1000 dernières mesures)
            if (usageHistory.size() > 1000) {
                usageHistory.erase(usageHistory.begin());
            }
        }
        
        this_thread::sleep_for(chrono::seconds(updateInterval.load()));
    }
}

// Mettre à jour l'utilisation actuelle
void CpuMonitor::updateCpuUsage() {
    vector<long> stats = readCpuStats();
    if (!stats.empty()) {
        double usage = calculateUsage(stats);
        currentUsage = usage;
    }
}

// Afficher les infos CPU
void CpuMonitor::displayCpuInfo() const {
    cout << "\n=== INFORMATIONS CPU ===" << endl;
    cout << cpuInfo;
    cout << "Utilisation actuelle: " << fixed << setprecision(2)
         << currentUsage.load() << "%" << endl;
    cout << "Intervalle: " << updateInterval.load() << "s" << endl;
    cout << "État: " << (isMonitoring.load() ? "ACTIF" : "INACTIF") << endl;
    
    {
        lock_guard<mutex> lock(historyMutex);
        cout << "Mesures enregistrées: " << usageHistory.size() << endl;
    }
    
    cout << "========================\n" << endl;
}

// Getters thread-safe
double CpuMonitor::getCurrentUsage() const {
    return currentUsage.load();
}

string CpuMonitor::getCpuInfo() const {
    return cpuInfo;
}

bool CpuMonitor::getMonitoringStatus() const {
    return isMonitoring.load();
}

int CpuMonitor::getUpdateInterval() const {
    return updateInterval.load();
}

// Setter amélioré
void CpuMonitor::setUpdateInterval(int interval) {
    if (interval > 0) {
        updateInterval = interval;
        cout << "Intervalle mis à jour: " << interval << "s" << endl;
    } else {
        cout << "Erreur: Intervalle invalide (doit être > 0)" << endl;
    }
}

// Exporter les données avec plus d'informations
void CpuMonitor::exportToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Erreur: Impossible d'écrire dans " << filename << endl;
        return;
    }

    // Obtenir le timestamp actuel
    auto now = chrono::system_clock::now();
    time_t time_now = chrono::system_clock::to_time_t(now);
    
    file << "=== RAPPORT CPU ===" << endl;
    file << "Timestamp: " << ctime(&time_now);
    file << cpuInfo;
    file << "Utilisation CPU actuelle: " << fixed << setprecision(2)
         << currentUsage.load() << "%" << endl;
    file << "Intervalle de surveillance: " << updateInterval.load() << "s" << endl;
    file << "État du monitoring: " << (isMonitoring.load() ? "ACTIF" : "INACTIF") << endl;
    file << endl;
    
    // Exporter l'historique
    {
        lock_guard<mutex> lock(historyMutex);
        if (!usageHistory.empty()) {
            file << "=== HISTORIQUE UTILISATION ===" << endl;
            for (size_t i = 0; i < usageHistory.size(); ++i) {
                file << "[" << i + 1 << "] " << fixed << setprecision(2)
                     << usageHistory[i] << "%" << endl;
            }
            
            // Statistiques
            auto minmax = minmax_element(usageHistory.begin(), usageHistory.end());
            double avg = 0.0;
            for (double val : usageHistory) avg += val;
            avg /= usageHistory.size();
            
            file << endl << "=== STATISTIQUES ===" << endl;
            file << "Minimum: " << fixed << setprecision(2) << *minmax.first << "%" << endl;
            file << "Maximum: " << fixed << setprecision(2) << *minmax.second << "%" << endl;
            file << "Moyenne: " << fixed << setprecision(2) << avg << "%" << endl;
        }
    }
    
    file.close();
    cout << "Données exportées vers: " << filename << endl;
}

// Historique d'utilisation thread-safe
void CpuMonitor::printUsageHistory() const {
    lock_guard<mutex> lock(historyMutex);
    
    cout << "\n=== HISTORIQUE UTILISATION CPU ===" << endl;
    if (usageHistory.empty()) {
        cout << "Aucune donnée disponible." << endl;
        return;
    }
    
    // Afficher les 20 dernières mesures au maximum
    size_t start = usageHistory.size() > 20 ? usageHistory.size() - 20 : 0;
    
    for (size_t i = start; i < usageHistory.size(); ++i) {
        cout << "[" << i + 1 << "] " << fixed << setprecision(2)
             << usageHistory[i] << "%" << endl;
    }
    
    if (usageHistory.size() > 20) {
        cout << "... (affichage des 20 dernières mesures sur " 
             << usageHistory.size() << " total)" << endl;
    }
    
    cout << "==================================\n" << endl;
}

// Formater l'affichage CPU avec couleurs
string CpuMonitor::formatUsageString() const {
    ostringstream oss;
    double usage = currentUsage.load();
    
    oss << "CPU: " << fixed << setprecision(1) << usage << "% [";
    
    int barLen = 20;
    int filled = static_cast<int>(usage / 100.0 * barLen);
    
    // Ajouter une barre de progression visuelle
    for (int i = 0; i < barLen; ++i) {
        if (i < filled) {
            // Couleur selon le niveau d'utilisation
            if (usage < 50) oss << "█";      // Normal
            else if (usage < 80) oss << "▓"; // Moyen
            else oss << "▒";                 // Élevé
        } else {
            oss << "░";
        }
    }
    
    oss << "] ";
    
    // Ajouter un indicateur de statut
    if (usage < 25) oss << "FAIBLE";
    else if (usage < 50) oss << "NORMAL";
    else if (usage < 75) oss << "ÉLEVÉ";
    else oss << "TRÈS ÉLEVÉ";
    
    return oss.str();
}

// Infos système améliorées (statique)
string CpuMonitor::getSystemInfo() {
    ostringstream info;
    info << "=== INFORMATIONS SYSTÈME ===" << endl;
    info << "Système: Linux" << endl;
    info << "Architecture: " << (sizeof(void*) == 8 ? "64-bit" : "32-bit") << endl;
    info << "Cœurs logiques: " << thread::hardware_concurrency() << endl;
    
    // Tenter d'obtenir plus d'infos système
    ifstream uptime("/proc/uptime");
    if (uptime.is_open()) {
        double uptimeSeconds;
        uptime >> uptimeSeconds;
        int hours = static_cast<int>(uptimeSeconds) / 3600;
        int minutes = (static_cast<int>(uptimeSeconds) % 3600) / 60;
        info << "Uptime: " << hours << "h " << minutes << "m" << endl;
        uptime.close();
    }
    
    info << "============================" << endl;
    return info.str();
}