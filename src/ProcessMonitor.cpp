#include "ProcessMonitor.h"

bool ProcessMonitor::update() { return true; }
ProcessMonitor::AP ProcessMonitor::getProcess(int) { return AP(); }
std::string ProcessMonitor::getProcessInfo() { return ""; }
