#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <thread>
#include <fstream>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <pdh.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "pdh.lib")
#elif __linux__
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <cstring>
#endif

class ProcessMonitor {
private:
    struct ProcessInfo {
        unsigned long pid;
        std::string name;
        double cpuUsage;
        size_t memoryUsage;    // en bytes
        size_t virtualMemory;  // en bytes
        std::string status;
        std::string user;
        long long cpuTime;     // pour calcul CPU
        long long lastCpuTime;
        std::chrono::steady_clock::time_point lastUpdateTime;
    };

    std::vector<ProcessInfo> processes;
    std::map<unsigned long, ProcessInfo> previousData;
    bool monitoring;
    int refreshInterval;

#ifdef _WIN32
    PDH_HQUERY cpuQuery;
    std::map<unsigned long, PDH_HCOUNTER> cpuCounters;
#endif

public:
    ProcessMonitor(int interval = 2000) : refreshInterval(interval), monitoring(false) {
#ifdef _WIN32
        PdhOpenQuery(NULL, NULL, &cpuQuery);
#endif
    }

    ~ProcessMonitor() {
#ifdef _WIN32
        PdhCloseQuery(cpuQuery);
#endif
    }

    // Convertir bytes en format lisible
    std::string formatBytes(size_t bytes) {
        const char* units[] = {"B", "KB", "MB", "GB", "TB"};
        int unitIndex = 0;
        double size = static_cast<double>(bytes);
        
        while (size >= 1024.0 && unitIndex < 4) {
            size /= 1024.0;
            unitIndex++;
        }
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
        return oss.str();
    }

    // Obtenir informations des processus
    bool getProcessList() {
        processes.clear();
        
#ifdef _WIN32
        return getProcessListWindows();
#elif __linux__
        return getProcessListLinux();
#endif
        return false;
    }

#ifdef _WIN32
    bool getProcessListWindows() {
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
            ProcessInfo info;
            info.pid = pe32.th32ProcessID;
            info.name = pe32.szExeFile;
            info.status = "Running";
            info.user = "System";

            // Obtenir informations mémoire
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
                                        FALSE, pe32.th32ProcessID);
            if (hProcess) {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    info.memoryUsage = pmc.WorkingSetSize;
                    info.virtualMemory = pmc.PagefileUsage;
                }

                // Obtenir temps CPU
                FILETIME creationTime, exitTime, kernelTime, userTime;
                if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) {
                    ULARGE_INTEGER kt, ut;
                    kt.LowPart = kernelTime.dwLowDateTime;
                    kt.HighPart = kernelTime.dwHighDateTime;
                    ut.LowPart = userTime.dwLowDateTime;
                    ut.HighPart = userTime.dwHighDateTime;
                    info.cpuTime = kt.QuadPart + ut.QuadPart;
                }

                CloseHandle(hProcess);
            }

            // Calculer usage CPU
            calculateCpuUsage(info);
            processes.push_back(info);

        } while (Process32Next(hProcessSnap, &pe32));

        CloseHandle(hProcessSnap);
        return true;
    }
#endif

#ifdef __linux__
    bool getProcessListLinux() {
        DIR* procDir = opendir("/proc");
        if (!procDir) return false;

        struct dirent* entry;
        while ((entry = readdir(procDir)) != nullptr) {
            // Vérifier si c'est un PID (numérique)
            if (strspn(entry->d_name, "0123456789") == strlen(entry->d_name)) {
                ProcessInfo info;
                info.pid = std::stoul(entry->d_name);
                
                if (getProcessInfoLinux(info)) {
                    calculateCpuUsage(info);
                    processes.push_back(info);
                }
            }
        }
        closedir(procDir);
        return true;
    }

    bool getProcessInfoLinux(ProcessInfo& info) {
        std::string statPath = "/proc/" + std::to_string(info.pid) + "/stat";
        std::string statusPath = "/proc/" + std::to_string(info.pid) + "/status";
        std::string cmdlinePath = "/proc/" + std::to_string(info.pid) + "/cmdline";

        // Lire /proc/[pid]/stat pour CPU et mémoire
        std::ifstream statFile(statPath);
        if (!statFile.is_open()) return false;

        std::string line;
        std::getline(statFile, line);
        std::istringstream iss(line);
        
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if (tokens.size() < 24) return false;

        // Extraire informations
        info.name = tokens[1];
        // Enlever parenthèses du nom
        if (info.name.front() == '(' && info.name.back() == ')') {
            info.name = info.name.substr(1, info.name.length() - 2);
        }

        info.status = tokens[2];
        
        // Temps CPU (user + system)
        long long utime = std::stoll(tokens[13]);
        long long stime = std::stoll(tokens[14]);
        info.cpuTime = utime + stime;

        // Mémoire virtuelle et résidente
        info.virtualMemory = std::stoull(tokens[22]);
        info.memoryUsage = std::stoull(tokens[23]) * getpagesize();

        // Lire informations additionnelles depuis /proc/[pid]/status
        std::ifstream statusFile(statusPath);
        if (statusFile.is_open()) {
            std::string statusLine;
            while (std::getline(statusFile, statusLine)) {
                if (statusLine.substr(0, 4) == "Uid:") {
                    info.user = "user"; // Simplification
                    break;
                }
            }
        }

        return true;
    }
#endif

    void calculateCpuUsage(ProcessInfo& info) {
        auto currentTime = std::chrono::steady_clock::now();
        info.cpuUsage = 0.0;

        if (previousData.find(info.pid) != previousData.end()) {
            ProcessInfo& prev = previousData[info.pid];
            
            auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>
                           (currentTime - prev.lastUpdateTime).count();
            
            if (timeDiff > 0) {
                long long cpuTimeDiff = info.cpuTime - prev.cpuTime;
                
#ifdef _WIN32
                // Windows: temps en 100ns units
                info.cpuUsage = (cpuTimeDiff * 100.0) / (timeDiff * 10000.0);
#elif __linux__
                // Linux: temps en clock ticks
                long int clockTicks = sysconf(_SC_CLK_TCK);
                info.cpuUsage = (cpuTimeDiff * 100.0) / (clockTicks * timeDiff / 1000.0);
#endif
                if (info.cpuUsage > 100.0) info.cpuUsage = 100.0;
                if (info.cpuUsage < 0.0) info.cpuUsage = 0.0;
            }
        }

        info.lastUpdateTime = currentTime;
        previousData[info.pid] = info;
    }

    // Afficher la liste des processus
    void displayProcesses(int count = 20, const std::string& sortBy = "cpu") {
        if (processes.empty()) {
            std::cout << "Aucun processus trouvé!" << std::endl;
            return;
        }

        // Trier les processus
        if (sortBy == "cpu") {
            std::sort(processes.begin(), processes.end(),
                     [](const ProcessInfo& a, const ProcessInfo& b) {
                         return a.cpuUsage > b.cpuUsage;
                     });
        } else if (sortBy == "memory") {
            std::sort(processes.begin(), processes.end(),
                     [](const ProcessInfo& a, const ProcessInfo& b) {
                         return a.memoryUsage > b.memoryUsage;
                     });
        } else if (sortBy == "name") {
            std::sort(processes.begin(), processes.end(),
                     [](const ProcessInfo& a, const ProcessInfo& b) {
                         return a.name < b.name;
                     });
        }

        // Affichage header
        std::cout << "\n" << std::string(90, '=') << std::endl;
        std::cout << "                    SURVEILLANCE DES PROCESSUS" << std::endl;
        std::cout << std::string(90, '=') << std::endl;
        
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        std::cout << "Mise à jour: " << std::ctime(&time_t) << std::endl;

        std::cout << std::left 
                  << std::setw(8) << "PID"
                  << std::setw(25) << "Nom Processus"
                  << std::setw(8) << "CPU %"
                  << std::setw(12) << "Mémoire"
                  << std::setw(12) << "Virtuelle"
                  << std::setw(10) << "Statut"
                  << std::setw(10) << "Utilisateur" << std::endl;
        
        std::cout << std::string(90, '-') << std::endl;

        // Afficher les processus
        int displayCount = std::min(count, static_cast<int>(processes.size()));
        for (int i = 0; i < displayCount; ++i) {
            const auto& proc = processes[i];
            
            std::string procName = proc.name;
            if (procName.length() > 24) {
                procName = procName.substr(0, 21) + "...";
            }

            std::cout << std::left
                      << std::setw(8) << proc.pid
                      << std::setw(25) << procName
                      << std::setw(8) << std::fixed << std::setprecision(1) << proc.cpuUsage
                      << std::setw(12) << formatBytes(proc.memoryUsage)
                      << std::setw(12) << formatBytes(proc.virtualMemory)
                      << std::setw(10) << proc.status.substr(0, 9)
                      << std::setw(10) << proc.user.substr(0, 9) << std::endl;
        }

        displaySummary();
    }

    void displaySummary() {
        if (processes.empty()) return;

        size_t totalMemory = 0;
        double totalCpu = 0.0;
        int activeProcesses = 0;

        for (const auto& proc : processes) {
            totalMemory += proc.memoryUsage;
            totalCpu += proc.cpuUsage;
            if (proc.status != "Z" && proc.status != "zombie") {
                activeProcesses++;
            }
        }

        std::cout << "\n" << std::string(50, '-') << std::endl;
        std::cout << "RÉSUMÉ:" << std::endl;
        std::cout << "Total processus: " << processes.size() << std::endl;
        std::cout << "Processus actifs: " << activeProcesses << std::endl;
        std::cout << "Mémoire totale utilisée: " << formatBytes(totalMemory) << std::endl;
        std::cout << "CPU total utilisé: " << std::fixed << std::setprecision(1) 
                  << totalCpu << "%" << std::endl;
    }

    // Rechercher un processus par nom
    void findProcess(const std::string& processName) {
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "RECHERCHE: " << processName << std::endl;
        std::cout << std::string(60, '=') << std::endl;

        bool found = false;
        for (const auto& proc : processes) {
            if (proc.name.find(processName) != std::string::npos) {
                if (!found) {
                    std::cout << std::left 
                              << std::setw(8) << "PID"
                              << std::setw(20) << "Nom"
                              << std::setw(8) << "CPU %"
                              << std::setw(12) << "Mémoire" << std::endl;
                    std::cout << std::string(50, '-') << std::endl;
                    found = true;
                }
                
                std::cout << std::left
                          << std::setw(8) << proc.pid
                          << std::setw(20) << proc.name
                          << std::setw(8) << std::fixed << std::setprecision(1) << proc.cpuUsage
                          << std::setw(12) << formatBytes(proc.memoryUsage) << std::endl;
            }
        }

        if (!found) {
            std::cout << "Aucun processus trouvé avec le nom: " << processName << std::endl;
        }
    }

    // Surveillance en temps réel
    void startRealTimeMonitoring() {
        monitoring = true;
        std::cout << "Démarrage surveillance temps réel..." << std::endl;
        std::cout << "Appuyez sur Ctrl+C pour arrêter" << std::endl;

        while (monitoring) {
            // Effacer écran
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif

            if (getProcessList()) {
                displayProcesses(25, "cpu");
            } else {
                std::cout << "Erreur lors de la récupération des processus!" << std::endl;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(refreshInterval));
        }
    }

    void stopMonitoring() {
        monitoring = false;
    }

    // Exporter rapport
    void exportReport(const std::string& filename = "process_report.txt") {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Erreur: impossible de créer le fichier " << filename << std::endl;
            return;
        }

        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        
        file << "RAPPORT DE SURVEILLANCE DES PROCESSUS" << std::endl;
        file << "Généré le: " << std::ctime(&time_t) << std::endl;
        file << std::string(80, '=') << std::endl;

        // Redirection temporaire de cout vers fichier
        std::streambuf* orig = std::cout.rdbuf();
        std::cout.rdbuf(file.rdbuf());

        displayProcesses(50, "cpu");

        std::cout.rdbuf(orig);
        file.close();

        std::cout << "Rapport exporté vers: " << filename << std::endl;
    }

    void setRefreshInterval(int interval) {
        refreshInterval = interval;
    }
};

// Interface utilisateur
class ProcessMonitorApp {
private:
    ProcessMonitor monitor;

public:
    void showMenu() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "          SURVEILLANCE DES PROCESSUS" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Afficher processus (triés par CPU)" << std::endl;
        std::cout << "2. Afficher processus (triés par mémoire)" << std::endl;
        std::cout << "3. Surveillance temps réel" << std::endl;
        std::cout << "4. Rechercher un processus" << std::endl;
        std::cout << "5. Exporter rapport" << std::endl;
        std::cout << "6. Configurer intervalle de mise à jour" << std::endl;
        std::cout << "7. Quitter" << std::endl;
        std::cout << std::string(50, '-') << std::endl;
        std::cout << "Votre choix (1-7): ";
    }

    void run() {
        int choice;
        std::string processName, filename;
        int interval;

        while (true) {
            showMenu();
            std::cin >> choice;

            switch (choice) {
                case 1:
                    if (monitor.getProcessList()) {
                        monitor.displayProcesses(20, "cpu");
                    }
                    break;
                
                case 2:
                    if (monitor.getProcessList()) {
                        monitor.displayProcesses(20, "memory");
                    }
                    break;
                
                case 3:
                    monitor.startRealTimeMonitoring();
                    break;
                
                case 4:
                    std::cout << "Entrez le nom du processus à rechercher: ";
                    std::cin.ignore();
                    std::getline(std::cin, processName);
                    if (monitor.getProcessList()) {
                        monitor.findProcess(processName);
                    }
                    break;
                
                case 5:
                    std::cout << "Nom du fichier (ou appuyez sur Entrée pour 'process_report.txt'): ";
                    std::cin.ignore();
                    std::getline(std::cin, filename);
                    if (filename.empty()) filename = "process_report.txt";
                    if (monitor.getProcessList()) {
                        monitor.exportReport(filename);
                    }
                    break;
                
                case 6:
                    std::cout << "Nouvel intervalle en millisecondes (défaut: 2000): ";
                    std::cin >> interval;
                    monitor.setRefreshInterval(interval);
                    std::cout << "Intervalle mis à jour: " << interval << "ms" << std::endl;
                    break;
                
                case 7:
                    std::cout << "Arrêt du moniteur de processus..." << std::endl;
                    return;
                
                default:
                    std::cout << "Choix invalide! Veuillez entrer 1-7." << std::endl;
            }

            if (choice != 3 && choice != 7) {
                std::cout << "\nAppuyez sur Entrée pour continuer...";
                std::cin.ignore();
                std::cin.get();
            }
        }
    }
};

int main() {
    try {
        std::cout << "=== MONITEUR DE PROCESSUS C++ ===" << std::endl;
        std::cout << "Surveillance CPU/RAM en temps réel" << std::endl;
        
        ProcessMonitorApp app;
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Erreur: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
