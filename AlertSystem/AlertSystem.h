#ifndef ALERT_SYSTEM_H
#define ALERT_SYSTEM_H
#include <string>


double getCpuUsage();
double getRamUsage();
void sendAlert(const std::string& message);
void monitorSystem();



#endif
