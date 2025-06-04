#include "ProcessMonitor.h"

bool ProcessMonitor::update()
{
    // Mettre à jour la liste des processus actifs
    return true;
}

AP ProcessMonitor::getProcess(int index)
{
    if (index >= 0 && index < activeProcesses.size())
    {
        return activeProcesses[index];
    }
    throw std::out_of_range("Processus non trouvé");
}

std::string ProcessMonitor::getProcessInfo()
{
    std::string info = "PID\tUtilisateur\tCPU%\tMémoire%\tChemin\n";
    for (const auto &process : activeProcesses)
    {
        info += std::to_string(process.PID) + "\t" + process.user + "\t" +
                std::to_string(process.cpu) + "\t" +
                std::to_string(process.memory) + "\t" + process.pathName + "\n";
    }
    return info;
}
