#ifndef CPUMONITOR_H
#define CPUMONITOR_H

class CpuMonitor {
public:
    CpuMonitor();
    double getUsage();

private:
    struct CpuTimes {
        unsigned long long user = 0, nice = 0, system = 0, idle = 0;
        unsigned long long iowait = 0, irq = 0, softirq = 0;

        unsigned long long total() const {
            return user + nice + system + idle + iowait + irq + softirq;
        }

        unsigned long long idleTime() const {
            return idle + iowait;
        }
    };

    CpuTimes readCpuTimes();
};

#endif // CPUMONITOR_H
