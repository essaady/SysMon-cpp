#ifndef SYSMON_H
#define SYSMON_H

#include "CpuMonitor.h"
#include "MemoryMonitor.h"
#include "ProcessMonitor.h"
#include <string>
using namespace std;

class SysMon {
private:
    int updateInter;    // Time in seconds between updates
    bool fullLog;           // Determines whether to log full data

public:
    SysMon();

    int run();                         
    string exportAsText() ;  
    string exportAsCSV() ;   

    bool update();                      // Trigger update for all modules
    string getTime() ;       
    string getInfo(const string& e) ;  
    void log() ;

};

#endif
