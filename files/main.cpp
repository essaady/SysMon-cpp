#include <iostream>
#include "MemoryMonitor.h"

int main() {
    MemoryMonitor mem_Monitor;
    
    std::cout << "Mémoire totale : " << mem_Monitor.getTotalMemory() << " MB\n";
    std::cout << "Mémoire utilisée : " << mem_Monitor.getUsedMemory() << " MB\n";

    return 0;
}
