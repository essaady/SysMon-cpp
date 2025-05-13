#include "..\include\CpuMonitor.h"
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>
using namespace std;

CpuMonitor::CpuMonitor() { readCpuStats(lastUser, lastNice, lastSystem, lastIdle); }

void CpuMonitor::readCpuStats(unsigned long long &user, unsigned long long &nice, unsigned long long &system, unsigned long long &idle) {
    ifstream file("/proc/stat");
    string line;
    getline(file, line);
    istringstream ss(line);
    string cpu;
    ss >> cpu >> user >> nice >> system >> idle;
}

float CpuMonitor::getCpuUsage() {
    unsigned long long user, nice, system, idle;
    readCpuStats(user, nice, system, idle);

    this_thread::sleep_for(chrono::milliseconds(100));

    unsigned long long user2, nice2, system2, idle2;
    readCpuStats(user2, nice2, system2, idle2);

    unsigned long long total1 = (user + nice + system + idle);
    unsigned long long total2 = (user2 + nice2 + system2 + idle2);

    unsigned long long totalDelta = total2 - total1;
    unsigned long long idleDelta = idle2 - idle;

    if (totalDelta == 0) return 0.0f;

    return 100.0f * (1.0f - ((float)idleDelta / totalDelta));
}
