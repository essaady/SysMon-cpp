#include "MemoryMonitor.h"

bool MemoryMonitor::update()
{
    // Mise à jour des informations mémoire
    return true;
}

unsigned long long MemoryMonitor::getTotalMemory()
{
    return totalMemMb;
}

unsigned long long MemoryMonitor::getFreeMemory()
{
    return freeMem;
}

unsigned long long MemoryMonitor::getTotalSwap()
{
    return SwapMemMb;
}

unsigned long long MemoryMonitor::getUsedSwap()
{
    return SwapMemMb - freeSwap;
}

double MemoryMonitor::getMemoryUsagePercentage()
{
    return (totalMemMb - freeMem) / (double)totalMemMb * 100;
}
