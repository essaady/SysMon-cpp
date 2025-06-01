#ifndef MEMORY_MONITOR_H
#define MEMORY_MONITOR_H

#include <string>
#include <map>

/**
 * @class MemoryMonitor
 * @brief Classe pour surveiller l'utilisation de la mémoire
 */
class MemoryMonitor {
public:
    /**
     * @brief Constructeur
     */
    MemoryMonitor();

    /**
     * @brief Destructeur
     */
    ~MemoryMonitor();

    /**
     * @brief Met à jour les informations de mémoire
     * @return true si la mise à jour a réussi, false sinon
     */
    bool update();

    /**
     * @brief Obtient la mémoire totale en kB
     * @return Mémoire totale
     */
    unsigned long long getTotalMemory() const;

    /**
     * @brief Obtient la mémoire libre en kB
     * @return Mémoire libre
     */
    unsigned long long getFreeMemory() const;

    /**
     * @brief Obtient la mémoire utilisée en kB
     * @return Mémoire utilisée
     */
    unsigned long long getUsedMemory() const;

    /**
     * @brief Obtient le pourcentage d'utilisation de la mémoire
     * @return Pourcentage d'utilisation (0-100)
     */
    double getMemoryUsagePercentage() const;

    /**
     * @brief Obtient la mémoire swap totale en kB
     * @return Swap total
     */
    unsigned long long getTotalSwap() const;

    /**
     * @brief Obtient la mémoire swap libre en kB
     * @return Swap libre
     */
    unsigned long long getFreeSwap() const;

    /**
     * @brief Obtient la mémoire swap utilisée en kB
     * @return Swap utilisé
     */
    unsigned long long getUsedSwap() const;

    /**
     * @brief Obtient le pourcentage d'utilisation du swap
     * @return Pourcentage d'utilisation (0-100)
     */
    double getSwapUsagePercentage() const;

    /**
     * @brief Exporte les données au format texte
     * @return Chaîne formatée des données mémoire
     */
    std::string exportAsText() const;

    /**
     * @brief Exporte les données au format CSV
     * @return Chaîne CSV des données mémoire
     */
    std::string exportAsCSV() const;

private:
    std::map<std::string, unsigned long long> memInfo;

    /**
     * @brief Lit les données mémoire depuis /proc/meminfo
     * @return true si la lecture a réussi, false sinon
     */
    bool readMemInfo();
};

#endif // MEMORY_MONITOR_H
