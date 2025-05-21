#ifndef PROCESS_MONITOR_H
#define PROCESS_MONITOR_H

#include <string>
#include <vector>
#include <map>

/**
 * @class ProcessInfo
 * @brief Structure contenant les informations d'un processus
 */
struct ProcessInfo {
    int pid;
    std::string name;
    std::string user;
    std::string state;
    double cpuUsage;
    double memoryUsage;
    unsigned long long rss;
    unsigned long long vsz;
    unsigned int priority;
    unsigned int nice;
    unsigned long long startTime;
    unsigned int threads;
};

/**
 * @class ProcessMonitor
 * @brief Classe pour surveiller les processus actifs
 */
class ProcessMonitor {
public:
    /**
     * @brief Constructeur
     */
    ProcessMonitor();

    /**
     * @brief Destructeur
     */
    ~ProcessMonitor();

    /**
     * @brief Met à jour la liste des processus
     * @return true si la mise à jour a réussi, false sinon
     */
    bool update();

    /**
     * @brief Obtient la liste des processus
     * @return Vecteur de ProcessInfo
     */
    std::vector<ProcessInfo> getProcesses() const;

    /**
     * @brief Obtient les N processus les plus consommateurs de CPU
     * @param n Nombre de processus à retourner
     * @return Vecteur de ProcessInfo
     */
    std::vector<ProcessInfo> getTopCPUProcesses(int n) const;

    /**
     * @brief Obtient les N processus les plus consommateurs de mémoire
     * @param n Nombre de processus à retourner
     * @return Vecteur de ProcessInfo
     */
    std::vector<ProcessInfo> getTopMemoryProcesses(int n) const;

    /**
     * @brief Obtient les informations d'un processus spécifique
     * @param pid ID du processus
     * @return ProcessInfo du processus demandé
     */
    ProcessInfo getProcessInfo(int pid) const;

    /**
     * @brief Exporte les données au format texte
     * @param limit Nombre maximum de processus à inclure (0 pour tous)
     * @return Chaîne formatée des données processus
     */
    std::string exportAsText(int limit = 0) const;

    /**
     * @brief Exporte les données au format CSV
     * @param limit Nombre maximum de processus à inclure (0 pour tous)
     * @return Chaîne CSV des données processus
     */
    std::string exportAsCSV(int limit = 0) const;

private:
    std::vector<ProcessInfo> processes;
    std::map<int, unsigned long long> prevCpuTimes;
    unsigned long long systemUptime;
    unsigned long long hertz;

    /**
     * @brief Lit les informations d'un processus
     * @param pid ID du processus
     * @return ProcessInfo du processus
     */
    ProcessInfo readProcessInfo(int pid);

    /**
     * @brief Obtient le nom d'utilisateur à partir de l'UID
     * @param uid UID de l'utilisateur
     * @return Nom d'utilisateur
     */
    std::string getUsernameFromUid(uid_t uid) const;

    /**
     * @brief Lit l'uptime du système
     * @return Uptime en secondes
     */
    unsigned long long readSystemUptime() const;

    /**
     * @brief Calcule l'utilisation CPU d'un processus
     * @param pid ID du processus
     * @param currentTime Temps CPU actuel
     * @return Pourcentage d'utilisation CPU
     */
    double calculateProcessCpuUsage(int pid, unsigned long long currentTime);
};

#endif // PROCESS_MONITOR_H