#include "CpuMonitor.h"

bool CpuMonitor::update()
{
    // Implémentation de la mise à jour des statistiques CPU
    return true;
}
CpuMonitor::CpuMonitor()
{
    // Initialisation si nécessaire
}
float CpuMonitor::getCpuUsage()
{
    return cpu.usagePerCPU;
}

float CpuMonitor::getCpuFreq()
{
    return cpu.frequency;
}

std::string CpuMonitor::getCpuInfo()
{
    return "CPU Info: " + std::to_string(cpu.nbrCPU) + " cores, Freq: " + std::to_string(cpu.frequency);
}
