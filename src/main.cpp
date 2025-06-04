#include <iostream>
#include "MemoryMonitor.h"

int main() {
    MemoryMonitor mem;

    if (!mem.update()) {
        std::cerr << "Erreur : impossible de lire /proc/meminfo" << std::endl;
        return 1;
    }

    std::cout << "=== Informations Mémoire ===" << std::endl;
    std::cout << "Mémoire totale (MB)      : " << mem.getTotalMemory() << std::endl;
    std::cout << "Mémoire libre (MB)       : " << mem.getFreeMemory() << std::endl;
    std::cout << "Utilisation mémoire (%)  : " << mem.getMemoryUsagePercentage() << "%" << std::endl;

    std::cout << "\n=== Informations Swap ===" << std::endl;
    std::cout << "Swap total (MB)          : " << mem.getTotalSwap() << std::endl;
    std::cout << "Swap utilisé (MB)        : " << mem.getUsedSwap() << std::endl;
    std::cout << "Utilisation swap (%)     : " << mem.getSwapUsagePercentage() << "%" << std::endl;

    return 0;
}
