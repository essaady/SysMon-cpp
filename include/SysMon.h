#ifndef SYSMON_H
#define SYSMON_H

#include <string>

class SysMon {
private:
    int updateInterval;   
    bool fullLog;         

public:
    SysMon(int interval = 1, bool log = false);
    ~SysMon();

    int run(); 

    std::string exportAsText();
    std::string exportAsCSV();

    bool update();
    std::string getTime();
    std::string getInfo(const std::string& type);
    void log();
};

#endif 
