#ifndef ProcessMon_H
#define ProcessMon_H

#include <string>

class ProcessMon {
    private:
    struct activeprocess {
        float cpu;
        float mem;
        struct Time {
            int h;
            int m;
            int s;
        };
        std::string user;
        std::string pathname;
        
    };
    int nbrprocess;
    activeprocess ap;
    public:
    ProcessMon();
    ~ProcessMon();
    bool update();
    activeprocess getProcess(int);
    std::string getProcessinfo();
    std::string getProcessraw();

};

#endif