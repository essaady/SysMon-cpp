#include "../include/CpuMonitor.h"
#include <cassert>

int main() {
    CpuMonitor cpu;
    assert(cpu.update());
    assert(cpu.getCpuUsage() >= 0.0f);
    assert(cpu.getCpuFreq() >= 0.0f);
    assert(cpu.getCpuInfo() >= 0.0f);
    return 0;
}
