#include "AlertSystem.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <cstdlib> //Il contient des fonctions standard pour gérer la mémoire, les opérations mathématiques et la conversion des données

using namespace std;

// fonction pour récupérer l'utilisation de  cpu depuis /proc/stat
double getCpuUsage() {
    ifstream file("/proc/stat");
    string line;
    getline(file, line);
    file.close();

    int user, nice, system, idle, iowait, irq, softirq, steal;
    sscanf(line.c_str(), "cpu %d %d %d %d %d %d %d %d", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);

    static int prevIdle = idle + iowait;
    static int prevTotal = user + nice + system + idle + iowait + irq + softirq + steal;

    int total = user + nice + system + idle + iowait + irq + softirq + steal;
    int idleTime = idle + iowait;

    int totalDelta = total - prevTotal;
    int idleDelta = idleTime - prevIdle;

    prevTotal = total;
    prevIdle = idleTime;

    return 100.0 * (totalDelta - idleDelta) / totalDelta;
}

//fonction pour récupérer l'utilisation de  RAM depuis /proc/meminfo
double getRamUsage() {
    ifstream file("/proc/meminfo");
    string key;
    int totalMem = 0, freeMem = 0;

    while (file>> key) {
        if (key == "MemTotal:") file>> totalMem;
        else if (key == "MemFree:") file>> freeMem;
}
    file.close();

    return 100.0 * (totalMem - freeMem) / totalMem;
}

//fct pour envoyer une alerte lorsqu'un seuil est dépassé
void sendAlert(const string& message) {
    string command = "notify-send 'ATENTION" + message + "'";
    system(command.c_str());
}

//fonction pour surveiller le système
void monitorSystem() {
    const double CPU_THRESHOLD = 90.0;  // Seuil d'alerte pour l'utilisation du CPU
    const double RAM_THRESHOLD = 80.0;  // Seuil d'alerte pour l'utilisation de la RAM

    while (true) {
        double cpuUsage = getCpuUsage();
        double ramUsage = getRamUsage();

        cout << "utilisation de CPU: " << cpuUsage << " %" << endl;
        cout << "utilisation de RAM: " << ramUsage << " %" << endl;
        cout << "----------------------------------\n";

        //envoi d'alerte si le seuil est dépassé
        if (cpuUsage> CPU_THRESHOLD) sendAlert("L'utilisation de CPU dépassée " + to_string(CPU_THRESHOLD) + "% !");
        if (ramUsage> RAM_THRESHOLD) sendAlert("L'utilisation de RAM dépassée " + to_string(RAM_THRESHOLD) + "% !");

        this_thread::sleep_for(chrono::seconds(2));
}
}
