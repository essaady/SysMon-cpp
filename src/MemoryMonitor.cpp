#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

class MoniteurMemoire {
public:
    static int memoireUtiliseeMo() {
        ifstream fichier("/proc/meminfo");
        string ligne;
        long memoireTotale = 0;
        long memoireLibre = 0;
        long tampons = 0;
        long cache = 0;

        while (getline(fichier, ligne)) {
            if (ligne.find("MemTotal:") == 0) {
                memoireTotale = extraireValeur(ligne);
            } else if (ligne.find("MemFree:") == 0) {
                memoireLibre = extraireValeur(ligne);
            } else if (ligne.find("Buffers:") == 0) {
                tampons = extraireValeur(ligne);
            } else if (ligne.find("Cached:") == 0) {
                cache = extraireValeur(ligne);
            }

            if (memoireTotale && memoireLibre && tampons && cache) {
                break;
            }
        }

        long utilisee = memoireTotale - memoireLibre - tampons - cache;
        return utilisee / 1024;
    }

private:
    static long extraireValeur(const string& ligne) {
        istringstream flux(ligne);
        string etiquette;
        long valeur;
        string unite;
        flux >> etiquette >> valeur >> unite;
        return valeur;
    }
};

int main() {
    int memoireUtilisee = MoniteurMemoire::memoireUtiliseeMo();
    cout << "Mémoire utilisée : " << memoireUtilisee << " Mo" << endl;
    return 0;
}
