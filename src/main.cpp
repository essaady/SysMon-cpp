#include <iostream>
#include <thread> //pour utiliser std::this_thread::sleep_for() (mise en pause du programme)
#include <chrono> // pour utiliser chrono dans sleep_for
#include "../include/MemoryMonitor.h"
#include "../include/CpuMonitor.h"
#include <iomanip>  //pour setprecision et fixed, voir https://devdocs.io/cpp/io/manip/setprecision et https://devdocs.io/cpp/io/manip/fixed

using namespace std;

int main() {
    MemoryMonitor memMonitor;

    cout << "SysMon-cpp - MEMORY MONITORING" << endl;
    cout << "------------------------------" << endl;

    //configuration de la precision pour tous les nombres flottants
    cout << fixed << setprecision(1);
    /*
    ou on peut utiliser aussi:
    cout.precision(1);
    cout<<fixed;
    mais sans #include <iomanip>, voir https://devdocs.io/cpp/io/ios_base/precision
    */
    while (true) {
        if (memMonitor.update()) {
            //convertir ram en GB
            const double totalRAM = memMonitor.getTotalMemory() / (1024.0 * 1024.0);
            const double usedRAM = memMonitor.getUsedMemory() / (1024.0 * 1024.0);
            const double freeRAM = memMonitor.getFreeMemory() / (1024.0 * 1024.0);

            //affichage de la ram
            cout << "___RAM___" << endl;
            cout << "Total RAM:    " << totalRAM << " GB" << endl;
            cout << "Used RAM:  " << usedRAM << " GB / " << totalRAM << " GB" << endl;
            cout << "Free RAM:    " << freeRAM << " GB / " << totalRAM << " GB" << endl;
            cout << "Use:    " << memMonitor.getMemoryUsagePercentage() << "%" << endl << endl;

            //convertir swap en GB
            const double totalSwap = memMonitor.getTotalSwap() / (1024.0 * 1024.0);
            const double usedSwap = memMonitor.getUsedSwap() / (1024.0 * 1024.0);
            const double freeSwap = memMonitor.getFreeSwap() / (1024.0 * 1024.0);

            //affichage swap
            if (totalSwap > 0) {
                cout << "___SWAP___" << endl;
                cout << "Total Swap:    " << totalSwap << " GB" << endl;
                cout << "Used Swap:  " << usedSwap << " GB / " << totalSwap << " GB" << endl;
                cout << "Free Swap:    " << freeSwap << " GB / " << totalSwap << " GB" << endl;
                cout << "Use:    " << memMonitor.getSwapUsagePercentage() << "%" << endl << endl;
            }

        } else {
            cout << "Erreur lors de la mise a jour des informations de memoire." << endl;
        }

        //attendre 2 secondes avant la prochaine mise a jour
        this_thread::sleep_for(chrono::seconds(2)); //l'utilisation de la fct chrono est naicessaire, voir la documentaion de la fct std::this_thread::sleep_for en https://devdocs.io/cpp/thread/sleep_for    }
        
        
        //_____CpuMonitor_____
        
        CpuMonitor monitor;
        monitor.update();
        cout << "___CPU INFO___" << endl;
        cout << monitor.getCpuInfo() << endl;
        

    return 0;
}
