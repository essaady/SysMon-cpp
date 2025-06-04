#ifndef SYSMON_H
#define SYSMON_H

#include <string>
#include <vector>

class SysMon {
private:
    int updateInterval;
    std::vector<std::string> fullLog;

public:
    SysMon();
    ~SysMon();

    int run();

    std::string exportAsText(const std::string& filepath);
    std::string exportAsCSV(const std::string& filepath);

    bool update();
    std::string getTime();
    std::string getInfo(std::string type);
    void log();
};

#endif
