#ifndef SYSMON_H
#define SYSMON_H

#include <string>

class SysMon {
private:
    int updateInterval;
    bool fullLog;

public:
  
    SysMon(int interval = 1, bool fullLog = false);

   
    int run();


    std::string exportAsText();
    std::string exportAsCSV();


    bool update();

    
    std::string getTime();
    std::string getInfo(std::string query);
    void log();
};

#endif
