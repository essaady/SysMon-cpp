#pragma once

#ifdef _WIN32
class CpuMonitor
{
public:
    double getCpuUsage();
};
#else
// تعريف نسخة Linux كما هي
#endif
