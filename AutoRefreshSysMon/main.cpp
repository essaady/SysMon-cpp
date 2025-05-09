#include <iostream>
#include <thread> // utiliser pour Parallel Execution
#include <chrono> // Pour contr�ler le timing et les d�lais dans le programme
using namespace std;
class InfoDisplayer {
private:
    // Les infos mises � jour
    int someData = 0;
public:
    void updateData() {
        someData++;
    }

    void displayInfo() const {
        cout << " update " << someData << endl;
    }

    void startRefreshLoop() {
        while (true) {

            updateData();
            displayInfo();

            // mis a jour apr�s deux seconds
            this_thread::sleep_for(chrono::seconds(2));
        }
    }
};

int main() {
    InfoDisplayer displayer;
    displayer.startRefreshLoop();

    return 0;
}
