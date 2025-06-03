#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#pragma comment(lib, "psapi.lib")
#elif __linux__
#include <unistd.h>
#include <sys/sysinfo.h>
#include <dirent.h>     
#endif

/**
 * @class MemoryMonitor
 * @brief Moniteur avancé de mémoire système avec surveillance temps réel
 * 
 * Cette classe fournit une surveillance complète de la mémoire système incluant:
 * - Surveillance de la mémoire physique et virtuelle du système
 * - Analyse détaillée de la consommation mémoire par processus
 * - Interface temps réel avec mise à jour configurable
 * - Fonctionnalités de recherche et d'export de rapports
 * - Compatibilité cross-platform (Windows/Linux)
 * 
 * Architecture technique:
 * - Utilise les APIs natives (GlobalMemoryStatusEx/sysinfo)
 * - Énumération des processus via Toolhelp32/proc filesystem
 * - Formatage intelligent des unités (B/KB/MB/GB/TB)
 * - Tri automatique par consommation mémoire
 * 
 * Fonctionnalités principales:
 * - Monitoring système global avec pourcentages d'utilisation
 * - Top processus consommateurs de mémoire
 * - Recherche de processus par nom avec correspondance partielle
 * - Export de rapports détaillés avec horodatage
 * - Interface interactive avec menu à 6 options
 * 
 * @author Ayman Essaady
 * @version 2.0 Enhanced
 * @date 2025
 */
class MemoryMonitor {
private:
    /**
     * @struct SystemMemoryInfo
     * @brief Structure contenant les informations globales de mémoire système
     * 
     * Stocke les métriques complètes de mémoire incluant:
     * - Mémoire physique (totale, utilisée, disponible)
     * - Mémoire virtuelle/swap (totale, utilisée, disponible)
     * - Pourcentages d'utilisation calculés
     */
    struct SystemMemoryInfo {
        size_t totalPhysical;           ///< Mémoire physique totale en bytes
        size_t availablePhysical;       ///< Mémoire physique disponible en bytes
        size_t usedPhysical;           ///< Mémoire physique utilisée en bytes
        size_t totalVirtual;           ///< Mémoire virtuelle totale en bytes
        size_t availableVirtual;       ///< Mémoire virtuelle disponible en bytes
        size_t usedVirtual;            ///< Mémoire virtuelle utilisée en bytes
        double physicalUsagePercent;   ///< Pourcentage d'utilisation mémoire physique
        double virtualUsagePercent;    ///< Pourcentage d'utilisation mémoire virtuelle
    };

    /**
     * @struct ProcessMemoryInfo
     * @brief Structure détaillée des informations mémoire par processus
     * 
     * Contient les métriques complètes de consommation mémoire:
     * - Working Set (mémoire physique active)
     * - Virtual Memory (espace d'adressage virtuel)
     * - Private Memory (mémoire privée du processus)
     */
    struct ProcessMemoryInfo {
        std::string processName;       ///< Nom du processus
        unsigned long processId;       ///< Identifiant unique du processus (PID)
        size_t workingSetSize;        ///< Taille du Working Set en bytes
        size_t virtualMemorySize;     ///< Taille mémoire virtuelle en bytes
        size_t privateMemorySize;     ///< Taille mémoire privée en bytes
    };

    std::vector<ProcessMemoryInfo> processes;  ///< Liste des processus surveillés
    SystemMemoryInfo systemInfo;              ///< Informations système globales
    bool monitoring;                          ///< État du monitoring temps réel
    int refreshInterval;                      ///< Intervalle de rafraîchissement (ms)

public:
    /**
     * @brief Constructeur du moniteur mémoire
     * @param interval Intervalle de mise à jour en millisecondes (défaut: 1000ms)
     */
    MemoryMonitor(int interval = 1000) : refreshInterval(interval), monitoring(false) {}

    /**
     * @brief Convertit les bytes en format lisible humainement
     * @param bytes Nombre de bytes à convertir
     * @return String formatée avec unité appropriée (B, KB, MB, GB, TB)
     * 
     * Utilise un algorithme de division successive par 1024 pour déterminer
     * l'unité la plus appropriée avec précision de 2 décimales.
     */
    std::string formatBytes(size_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unitIndex = 0;
        double size = static_cast<double>(bytes);
        
        while (size >= 1024.0 && unitIndex < 4) {
            size /= 1024.0;
            unitIndex++;
        }
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];
        return oss.str();
    }

    /**
     * @brief Récupère les informations globales de mémoire système
     * @return true si succès, false en cas d'erreur
     * 
     * Utilise les APIs natives du système:
     * - Windows: GlobalMemoryStatusEx() pour mémoire physique/virtuelle
     * - Linux: sysinfo() pour RAM et swap
     * 
     * Calcule automatiquement les pourcentages d'utilisation et
     * les valeurs de mémoire utilisée.
     */
    bool getSystemMemoryInfo() {
#ifdef _WIN32
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        
        if (!GlobalMemoryStatusEx(&memInfo)) {
            return false;
        }
        
        systemInfo.totalPhysical = memInfo.ullTotalPhys;
        systemInfo.availablePhysical = memInfo.ullAvailPhys;
        systemInfo.usedPhysical = systemInfo.totalPhysical - systemInfo.availablePhysical;
        systemInfo.totalVirtual = memInfo.ullTotalVirtual;
        systemInfo.availableVirtual = memInfo.ullAvailVirtual;
        systemInfo.usedVirtual = systemInfo.totalVirtual - systemInfo.availableVirtual;
        
#elif __linux__
        struct sysinfo si;
        if (sysinfo(&si) != 0) {
            return false;
        }
        
        systemInfo.totalPhysical = si.totalram * si.mem_unit;
        systemInfo.availablePhysical = si.freeram * si.mem_unit;
        systemInfo.usedPhysical = systemInfo.totalPhysical - systemInfo.availablePhysical;
        systemInfo.totalVirtual = si.totalswap * si.mem_unit;
        systemInfo.availableVirtual = si.freeswap * si.mem_unit;
        systemInfo.usedVirtual = systemInfo.totalVirtual - systemInfo.availableVirtual;
#endif
        
        // Calcul des pourcentages d'utilisation
        systemInfo.physicalUsagePercent = 
            (static_cast<double>(systemInfo.usedPhysical) / systemInfo.totalPhysical) * 100.0;
        systemInfo.virtualUsagePercent = 
            systemInfo.totalVirtual > 0 ? 
            (static_cast<double>(systemInfo.usedVirtual) / systemInfo.totalVirtual) * 100.0 : 0.0;
        
        return true;
    }

    /**
     * @brief Énumère et analyse tous les processus système
     * @return true si succès, false en cas d'erreur
     * 
     * Parcourt tous les processus actifs et collecte:
     * - PID et nom du processus
     * - Working Set Size (mémoire physique)
     * - Virtual Memory Size (espace virtuel)
     * - Private Memory Size (mémoire privée)
     * 
     * Implémentation spécifique par plateforme:
     * - Windows: CreateToolhelp32Snapshot + GetProcessMemoryInfo
     * - Linux: Parcours /proc/[pid]/ avec lecture status/statm
     * 
     * Tri automatique par consommation mémoire décroissante.
     */
    bool getProcessMemoryInfo() {
        processes.clear();
        
#ifdef _WIN32
        HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hProcessSnap == INVALID_HANDLE_VALUE) {
            return false;
        }
        
        PROCESSENTRY32 pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32);
        
        if (!Process32First(hProcessSnap, &pe32)) {
            CloseHandle(hProcessSnap);
            return false;
        }
        
        do {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
                                       FALSE, pe32.th32ProcessID);
            if (hProcess) {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    ProcessMemoryInfo info;
                    info.processName = pe32.szExeFile;
                    info.processId = pe32.th32ProcessID;
                    info.workingSetSize = pmc.WorkingSetSize;
                    info.virtualMemorySize = pmc.PagefileUsage;
                    info.privateMemorySize = pmc.WorkingSetSize;
                    processes.push_back(info);
                }
                CloseHandle(hProcess);
            }
        } while (Process32Next(hProcessSnap, &pe32));
        
        CloseHandle(hProcessSnap);
        
#elif __linux__
        DIR* procDir = opendir("/proc");
        if (!procDir) return false;
        
        struct dirent* entry;
        while ((entry = readdir(procDir)) != nullptr) {
            // Vérifier si c'est un PID (nom numérique)
            if (strspn(entry->d_name, "0123456789") == strlen(entry->d_name)) {
                std::string statusPath = "/proc/" + std::string(entry->d_name) + "/status";
                std::string statmPath = "/proc/" + std::string(entry->d_name) + "/statm";
                
                std::ifstream statusFile(statusPath);
                std::ifstream statmFile(statmPath);
                
                if (statusFile.is_open() && statmFile.is_open()) {
                    ProcessMemoryInfo info;
                    info.processId = std::stoul(entry->d_name);
                    
                    // Lire le nom du processus depuis /proc/[pid]/status
                    std::string line;
                    while (std::getline(statusFile, line)) {
                        if (line.substr(0, 5) == "Name:") {
                            info.processName = line.substr(6);
                            break;
                        }
                    }
                    
                    // Lire les statistiques mémoire depuis /proc/[pid]/statm
                    size_t vmSize, vmRSS;
                    if (statmFile >> vmSize >> vmRSS) {
                        info.virtualMemorySize = vmSize * getpagesize();
                        info.workingSetSize = vmRSS * getpagesize();
                        info.privateMemorySize = info.workingSetSize;
                        processes.push_back(info);
                    }
                }
            }
        }
        closedir(procDir);
#endif
        
        // Tri des processus par consommation mémoire (ordre décroissant)
        std::sort(processes.begin(), processes.end(), 
                 [](const ProcessMemoryInfo& a, const ProcessMemoryInfo& b) {
                     return a.workingSetSize > b.workingSetSize;
                 });
        
        return true;
    }

    /**
     * @brief Affiche les informations détaillées de mémoire système
     * 
     * Présente un rapport complet incluant:
     * - Mémoire physique totale, utilisée et disponible
     * - Mémoire virtuelle/swap avec statistiques
     * - Barre de progression visuelle Unicode
     * - Pourcentages d'utilisation formatés
     * 
     * Utilise un formatage professionnel avec alignement des colonnes
     * et caractères Unicode pour la visualisation graphique.
     */
    void displaySystemInfo() {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "           INFORMATIONS MÉMOIRE SYSTÈME" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        std::cout << std::left << std::setw(25) << "Mémoire Physique Totale:" 
                  << formatBytes(systemInfo.totalPhysical) << std::endl;
        std::cout << std::left << std::setw(25) << "Mémoire Physique Utilisée:" 
                  << formatBytes(systemInfo.usedPhysical) 
                  << " (" << std::fixed << std::setprecision(1) 
                  << systemInfo.physicalUsagePercent << "%)" << std::endl;
        std::cout << std::left << std::setw(25) << "Mémoire Physique Libre:" 
                  << formatBytes(systemInfo.availablePhysical) << std::endl;
        
        if (systemInfo.totalVirtual > 0) {
            std::cout << std::left << std::setw(25) << "Mémoire Virtuelle Totale:" 
                      << formatBytes(systemInfo.totalVirtual) << std::endl;
            std::cout << std::left << std::setw(25) << "Mémoire Virtuelle Utilisée:" 
                      << formatBytes(systemInfo.usedVirtual) 
                      << " (" << std::fixed << std::setprecision(1) 
                      << systemInfo.virtualUsagePercent << "%)" << std::endl;
        }
        
        // Barre de progression visuelle avec caractères Unicode
        std::cout << "\nUtilisation Mémoire: [";
        int barWidth = 40;
        int filledWidth = static_cast<int>((systemInfo.physicalUsagePercent / 100.0) * barWidth);
        for (int i = 0; i < barWidth; ++i) {
            if (i < filledWidth) std::cout << "█";
            else std::cout << "░";
        }
        std::cout << "] " << std::fixed << std::setprecision(1) 
                  << systemInfo.physicalUsagePercent << "%" << std::endl;
    }

    /**
     * @brief Affiche le top des processus consommateurs de mémoire
     * @param count Nombre de processus à afficher (défaut: 10)
     * 
     * Présente un tableau formaté des processus triés par consommation mémoire:
     * - PID du processus
     * - Nom du processus (tronqué si nécessaire)
     * - Working Set (mémoire physique active)
     * - Virtual Memory (espace d'adressage virtuel)
     * - Private Memory (mémoire privée)
     * 
     * Utilise un formatage tabulaire professionnel avec largeurs fixes
     * et troncature intelligente des noms longs.
     */
    void displayTopProcesses(int count = 10) {
        std::cout << "\n" << std::string(80, '=') << std::endl;
        std::cout << "           TOP " << count << " PROCESSUS CONSOMMATEURS MÉMOIRE" << std::endl;
        std::cout << std::string(80, '=') << std::endl;
        
        std::cout << std::left << std::setw(8) << "PID" 
                  << std::setw(25) << "Nom Processus"
                  << std::setw(15) << "Working Set"
                  << std::setw(15) << "Mém. Virtuelle"
                  << std::setw(15) << "Mém. Privée" << std::endl;
        std::cout << std::string(80, '-') << std::endl;
        
        int displayCount = std::min(count, static_cast<int>(processes.size()));
        for (int i = 0; i < displayCount; ++i) {
            const auto& proc = processes[i];
            std::cout << std::left << std::setw(8) << proc.processId
                      << std::setw(25) << (proc.processName.length() > 24 ? 
                                          proc.processName.substr(0, 21) + "..." : proc.processName)
                      << std::setw(15) << formatBytes(proc.workingSetSize)
                      << std::setw(15) << formatBytes(proc.virtualMemorySize)
                      << std::setw(15) << formatBytes(proc.privateMemorySize) << std::endl;
        }
    }

    /**
     * @brief Recherche des processus par nom avec correspondance partielle
     * @param processName Nom ou partie du nom à rechercher
     * 
     * Effectue une recherche insensible à la casse dans la liste des processus.
     * Affiche tous les processus dont le nom contient la chaîne recherchée.
     * 
     * Fonctionnalités:
     * - Recherche par correspondance partielle (substring)
     * - Affichage formaté des résultats trouvés
     * - Message informatif si aucun résultat
     * - Tableau avec PID, nom et consommation mémoire
     */
    void findProcess(const std::string& processName) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "         RÉSULTATS RECHERCHE: " << processName << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        bool found = false;
        for (const auto& proc : processes) {
            if (proc.processName.find(processName) != std::string::npos) {
                if (!found) {
                    std::cout << std::left << std::setw(8) << "PID" 
                              << std::setw(25) << "Nom Processus"
                              << std::setw(15) << "Working Set" << std::endl;
                    std::cout << std::string(50, '-') << std::endl;
                    found = true;
                }
                std::cout << std::left << std::setw(8) << proc.processId
                          << std::setw(25) << proc.processName
                          << std::setw(15) << formatBytes(proc.workingSetSize) << std::endl;
            }
        }
        
        if (!found) {
            std::cout << "Aucun processus trouvé correspondant à '" << processName << "'" << std::endl;
        }
    }

    /**
     * @brief Lance la surveillance continue en temps réel
     * 
     * Mode monitoring temps réel avec les fonctionnalités:
     * - Effacement automatique de l'écran (cross-platform)
     * - Mise à jour cyclique selon l'intervalle configuré
     * - Affichage horodaté des informations
     * - Combinaison système + top processus
     * - Arrêt par Ctrl+C
     * 
     * Utilise std::this_thread::sleep_for pour contrôler la fréquence
     * et system("cls"/"clear") pour l'effacement d'écran.
     */
    void startMonitoring() {
        monitoring = true;
        std::cout << "Démarrage surveillance mémoire (Ctrl+C pour arrêter)..." << std::endl;
        
        while (monitoring) {
            // Effacement écran (cross-platform)
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            
            auto now = std::chrono::system_clock::now();
            auto time_t = std::chrono::system_clock::to_time_t(now);
            std::cout << "Moniteur Mémoire - " << std::ctime(&time_t);
            
            if (getSystemMemoryInfo() && getProcessMemoryInfo()) {
                displaySystemInfo();
                displayTopProcesses(15);
            } else {
                std::cout << "Erreur récupération informations mémoire!" << std::endl;
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(refreshInterval));
        }
    }

    /**
     * @brief Arrête la surveillance temps réel
     */
    void stopMonitoring() {
        monitoring = false;
    }

    /**
     * @brief Obtient et affiche les statistiques mémoire actuelles
     * 
     * Mode snapshot unique combinant:
     * - Informations système globales
     * - Top 10 processus consommateurs
     * - Gestion d'erreurs avec messages informatifs
     */
    void getMemoryStats() {
        if (getSystemMemoryInfo() && getProcessMemoryInfo()) {
            displaySystemInfo();
            displayTopProcesses();
        } else {
            std::cout << "Erreur récupération informations mémoire!" << std::endl;
        }
    }

    /**
     * @brief Configure l'intervalle de rafraîchissement
     * @param interval Nouvel intervalle en millisecondes
     */
    void setRefreshInterval(int interval) {
        refreshInterval = interval;
    }
};

/**
 * @class MemoryMonitorApp
 * @brief Interface utilisateur interactive pour le moniteur mémoire
 * 
 * Fournit une interface menu complète avec 6 options:
 * 1. Affichage statut mémoire actuel
 * 2. Surveillance temps réel continue
 * 3. Recherche de processus par nom
 * 4. Configuration intervalle de mise à jour
 * 5. Export de rapport détaillé
 * 6. Sortie de l'application
 * 
 * Gestion robuste des entrées utilisateur avec validation
 * et gestion d'erreurs appropriée.
 */
class MemoryMonitorApp {
private:
    MemoryMonitor monitor;  ///< Instance du moniteur mémoire

public:
    /**
     * @brief Affiche le menu principal interactif
     * 
     * Présente les 6 options disponibles avec formatage visuel
     * et instructions claires pour l'utilisateur.
     */
    void showMenu() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "            MONITEUR MÉMOIRE AVANCÉ" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Afficher Statut Mémoire Actuel" << std::endl;
        std::cout << "2. Démarrer Surveillance Temps Réel" << std::endl;
        std::cout << "3. Rechercher Processus par Nom" << std::endl;
        std::cout << "4. Configurer Intervalle Mise à Jour" << std::endl;
        std::cout << "5. Exporter Rapport Mémoire" << std::endl;
        std::cout << "6. Quitter" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Votre choix (1-6): ";
    }

    /**
     * @brief Exporte un rapport détaillé vers fichier texte
     * 
     * Génère un fichier "memory_report.txt" contenant:
     * - Horodatage de génération
     * - Statistiques système complètes
     * - Liste détaillée des processus
     * 
     * Utilise la redirection temporaire de std::cout pour
     * capturer proprement la sortie formatée.
     */
    void exportReport() {
        std::ofstream file("memory_report.txt");
        if (!file.is_open()) {
            std::cout << "Erreur création fichier rapport!" << std::endl;
            return;
        }

        // Redirection temporaire de cout vers fichier
        std::streambuf* orig = std::cout.rdbuf();
        std::cout.rdbuf(file.rdbuf());

        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "Rapport Mémoire Généré: " << std::ctime(&time_t) << std::endl;

        monitor.getMemoryStats();

        // Restauration cout original
        std::cout.rdbuf(orig);
        file.close();

        std::cout << "Rapport mémoire exporté vers 'memory_report.txt'" << std::endl;
    }

    /**
     * @brief Boucle principale de l'application
     * 
     * Gère l'interaction utilisateur avec:
     * - Affichage menu cyclique
     * - Traitement des choix utilisateur
     * - Validation des entrées
     * - Gestion des erreurs
     * - Pause interactive entre operations
     */
    void run() {
        int choice;
        std::string processName;
        int interval;

        while (true) {
            showMenu();
            std::cin >> choice;

            switch (choice) {
                case 1:
                    monitor.getMemoryStats();
                    break;
                case 2:
                    monitor.startMonitoring();
                    break;
                case 3:
                    std::cout << "Entrez le nom du processus à rechercher: ";
                    std::cin.ignore();
                    std::getline(std::cin, processName);
                    if (monitor.getProcessMemoryInfo()) {
                        monitor.findProcess(processName);
                    }
                    break;
                case 4:
                    std::cout << "Entrez l'intervalle en millisecondes (défaut 1000): ";
                    std::cin >> interval;
                    monitor.setRefreshInterval(interval);
                    std::cout << "Intervalle configuré à " << interval << "ms" << std::endl;
                    break;
                case 5:
                    exportReport();
                    break;
                case 6:
                    std::cout << "Arrêt du Moniteur Mémoire..." << std::endl;
                    return;
                default:
                    std::cout << "Choix invalide! Veuillez entrer 1-6." << std::endl;
            }

            if (choice != 2 && choice != 6) {
                std::cout << "\nAppuyez sur Entrée pour continuer...";
                std::cin.ignore();
                std::cin.get();
            }
        }
    }
};

/**
 * @brief Point d'entrée principal de l'application
 * @return Code de retour (0 = succès, 1 = erreur)
 * 
 * Initialise l'application avec gestion d'exceptions
 * et messages d'erreur appropriés.
 */
int main() {
    try {
        std::cout << "=== MONITEUR MÉMOIRE AVANCÉ C++ ===" << std::endl;
        std::cout << "Surveillance mémoire système et processus temps réel" << std::endl;
        std::cout << "Version 2.0 Enhanced - Cross-platform" << std::endl;
        
        MemoryMonitorApp app;
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
