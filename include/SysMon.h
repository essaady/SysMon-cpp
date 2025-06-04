#ifndef SYSMON_H
#define SYSMON_H

#include <string>

class SysMon {
private:
    int updateInterval;
    std::string fullLog;

public:
    SysMon(int interval = 2); // constructeur avec intervalle par défaut

    int run();
    std::string exportAsText();
    std::string exportAsCSV();
    bool update();
    std::string getTime();
    std::string getInfo(const std::string& format);
    void log();
};

#endif
