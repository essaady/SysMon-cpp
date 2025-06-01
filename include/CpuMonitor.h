#ifndef CPU_MONITOR_H
#define CPU_MONITOR_H

#include <string>
#include <vector>
#include <map>

/**
 * @class CpuMonitor
 * @brief Classe pour surveiller l'utilisation du CPU
 */
class CpuMonitor {
public:
    /**
     * @brief Constructeur
     */
    CpuMonitor();

    /**
     * @brief Destructeur
     */
    ~CpuMonitor();

    /**
     * @brief Met à jour les informations du CPU
     * @return true si la mise à jour a réussi, false sinon
     */
    bool update();

    /**
     * @brief Obtient le pourcentage d'utilisation total du CPU
     * @return Pourcentage d'utilisation (0-100)
     */
    double getTotalUsage() const;

    /**
     * @brief Obtient le pourcentage d'utilisation par cœur
     * @return Map des pourcentages d'utilisation par cœur
     */
    std::map<std::string, double> getCoreUsage() const;

    /**
     * @brief Exporte les données au format texte
     * @return Chaîne formatée des données CPU
     */
    std::string exportAsText() const;

    /**
     * @brief Exporte les données au format CSV
     * @return Chaîne CSV des données CPU
     */
    std::string exportAsCSV() const;

private:
    struct CPUData {
        unsigned long long user;
        unsigned long long nice;
        unsigned long long system;
        unsigned long long idle;
        unsigned long long iowait;
        unsigned long long irq;
        unsigned long long softirq;
        unsigned long long steal;
        unsigned long long guest;
        unsigned long long guest_nice;
    };

    std::map<std::string, CPUData> prevCpuData;
    std::map<std::string, CPUData> currentCpuData;
    std::map<std::string, double> cpuUsage;

    /**
     * @brief Calcule l'utilisation du CPU entre deux mesures
     * @param prev Données CPU précédentes
     * @param current Données CPU actuelles
     * @return Pourcentage d'utilisation
     */
    double calculateUsage(const CPUData& prev, const CPUData& current) const;

    /**
     * @brief Lit les données CPU depuis /proc/stat
     * @return true si la lecture a réussi, false sinon
     */
    bool readCpuData();
};

#endif // CPU_MONITOR_H
