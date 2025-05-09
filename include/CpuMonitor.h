#ifndef CPUMONITOR_H
#define CPUMONITOR_H

class CpuMonitor {
public:
    double getUsage();

private:
    struct CpuSnapshot {
        unsigned long long user, nice, system, idle;
        unsigned long long iowait, irq, softirq;

        unsigned long long total() const;
        unsigned long long idleTime() const;
    };

    CpuSnapshot takeSnapshot() const;
};

#endif // CPUMONITOR_H
