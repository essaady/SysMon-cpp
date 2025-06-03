#include "ProcessMonitor.h"
#include <iostream>

// constructeur
ProcessMonitor::ProcessMonitor() {
    nbrProcess = 1;
    PID = new int[nbrProcess];
    PID[0] = 1234; // PID fictif
}

// destructeur
ProcessMonitor::~ProcessMonitor() {
    delete[] PID;
}

// remplir les infos du processus (test)
bool ProcessMonitor::update() {
    AP.cpu = 15.2;
    AP.memory = 512.0;
    AP.user = "etudiant";
    AP.pathName = "/usr/bin/demo";
    return true;
}

// retourne le processus (on a qu un seul ici)
activeProcesses ProcessMonitor::getProcess(int index) {
    return AP;
}

// retourne les infos format texte
string ProcessMonitor::getProcessInfo() {
    string info = "PID: " + to_string(PID[0]) +
                  ", User: " + AP.user +
                  ", CPU: " + to_string(AP.cpu) +
                  ", RAM: " + to_string(AP.memory) +
                  ", Path: " + AP.pathName;
    return info;
}

// retourne les infos brutes
string ProcessMonitor::getProcessRaw() {
    string raw = to_string(PID[0]) + "," +
                 to_string(AP.cpu) + "," +
                 to_string(AP.memory) + "," +
                 AP.user + "," +
                 AP.pathName;
    return raw;
}

// Fin de fichier hProcessMonitpr.cpp