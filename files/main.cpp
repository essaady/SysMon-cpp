#include <iostream>
#include "MemoryMonitor.h"

int main() {
    MemoryMonitor mem_Monitor;
    
    std::cout << "M�moire totale : " << mem_Monitor.getTotalMemory() << " MB\n";
    std::cout << "M�moire utilis�e : " << mem_Monitor.getUsedMemory() << " MB\n";

    return 0;
}
