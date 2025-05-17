
#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <string>
#include <chrono>

/**
 * @enum ExportFormat
 * @brief Format d'exportation des données
 */
enum class ExportFormat {
    TEXT,
    CSV
};

/**
 * @class SysMon
 * @brief Classe principale coordonnant les différents moniteurs
 */
class SysMon {
public:
    /**
     * @brief Constructeur
     * @param updateInterval Intervalle de mise à jour en millisecondes
     */
    SysMon(int updateInterval = 1000);

    /**
     * @brief Destructeur
     */
    ~SysMon();

    /**
     * @brief Démarre la surveillance
     */
    void start();

    /**
     * @brief Arrête la surveillance
     */
    void stop();

    /**
     * @brief Met à jour toutes les informations système
     * @return true si la mise à jour a réussi, false sinon
     */
    bool update();

    /**
     * @brief Affiche les informations système dans la console
     * @param showProcessCount Nombre de processus à afficher (0 pour tous)
     */
    void display(int showProcessCount = 10) const;

    /**
     * @brief Exporte les données système
     * @param filename Nom du fichier de sortie
     * @param format Format d'exportation
     * @param showProcessCount Nombre de processus à inclure (0 pour tous)
     * @return true si l'exportation a réussi, false sinon
     */
    bool exportData(const std::string& filename, ExportFormat format, int showProcessCount = 0) const;

    /**
     * @brief Obtient le moniteur CPU
     * @return Référence vers le moniteur CPU
     */
    const CpuMonitor& getCpuMonitor() const;

    /**
     * @brief Obtient le moniteur mémoire
     * @return Référence vers le moniteur mémoire
     */
    const MemoryMonitor& getMemoryMonitor() const;

    /**
     * @brief Obtient le moniteur processus
     * @return Référence vers le moniteur processus
     */
    const ProcessMonitor& getProcessMonitor() const;

    /**
     * @brief Définit l'intervalle de mise à jour
     * @param interval Intervalle en millisecondes
     */
    void setUpdateInterval(int interval);

    /**
     * @brief Obtient l'intervalle de mise à jour
     * @return Intervalle en millisecondes
     */
    int getUpdateInterval() const;

private:
    CpuMonitor cpuMonitor;
    MemoryMonitor memoryMonitor;
    ProcessMonitor processMonitor;
    int updateInterval;
    bool running;
    std::chrono::time_point<std::chrono::steady_clock> lastUpdateTime;

    /**
     * @brief Boucle principale de surveillance
     */
    void monitorLoop();
};

#endif // SYSMON_H

