#include <iostream>
#include "../include/MemoryMonitor.h"
using namespace std;

int main() {
    
MemoryMonitor monitor;
    cout << "SysMon-cpp -Moniteur de memoire" << endl;

if (!monitor.update()) {
    cout << "Mise a jour:" << endl;

return 1;
}
monitor.display();
    cout << "Erreur " <<endl;

return 0;

}