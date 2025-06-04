#include <iostream>
#include <string>
#include <vector>
#include <ctime> //pour date et heur
using namespace std;

class SysMon {
private:
    int updateInterval;
    vector<string> fullLog;

public:
    // Constructeur
    SysMon(int interval = 5) :  updateInterval(interval) {}

    // exécute le programme 
    int run() {
        log(); //capter le début de l'exécution 
        update();
        return 0; // réinitialiser a la fin si réussit à 0 
    }

    // Convertir en text
    string exportAsText() {
        string result;
        for (const auto& entry : fullLog)
            result += entry + "\n";
        return result;
    }

    // CSV
    string exportAsCSV() {
        string result;
        for (const auto& entry : fullLog)
            result += entry + ",\n";
        return result;
    }

    // update des données 
    bool update() {
        fullLog.push_back("Updated at " + getTime());
        return true;
    }

    // return la date actuelle 
    string getTime() {
        time_t now = time(0);
        return 
string(ctime(&now));
    }

    // return les données à partir du nom
    string getInfo(string name) {
        return "Info about " + name;
    }

    void log() {
        fullLog.push_back("SysMon started at " + getTime());
    }
};