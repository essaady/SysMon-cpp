#ifndef SysMonitor_H
#define SysMonitor_H

#include <string>

class Sysmon {
    int updateinterval;
    std::string fulllog;

    public :
    Sysmon();
    ~Sysmon();
    std::string exportastxt();
    std::string exportascsv();
    bool update();
    std::string getTime();
    std::string getinfo(std::string);
    void log();

};

#endif
