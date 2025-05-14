#ifdef _WIN32
#include <windows.h>
#include <iostream>

class CpuMonitor
{
public:
    double getCpuUsage()
    {
        static FILETIME prevIdleTime, prevKernelTime, prevUserTime;
        FILETIME idleTime, kernelTime, userTime;

        if (!GetSystemTimes(&idleTime, &kernelTime, &userTime))
        {
            std::cerr << "Failed to get CPU times.\n";
            return -1;
        }

        ULONGLONG idleDiff = subtractTimes(idleTime, prevIdleTime);
        ULONGLONG kernelDiff = subtractTimes(kernelTime, prevKernelTime);
        ULONGLONG userDiff = subtractTimes(userTime, prevUserTime);

        ULONGLONG total = kernelDiff + userDiff;

        prevIdleTime = idleTime;
        prevKernelTime = kernelTime;
        prevUserTime = userTime;

        if (total == 0)
            return 0.0;
        return (1.0 - ((double)idleDiff / total)) * 100.0;
    }

private:
    ULONGLONG subtractTimes(const FILETIME &a, const FILETIME &b)
    {
        ULARGE_INTEGER ua, ub;
        ua.LowPart = a.dwLowDateTime;
        ua.HighPart = a.dwHighDateTime;
        ub.LowPart = b.dwLowDateTime;
        ub.HighPart = b.dwHighDateTime;
        return ua.QuadPart - ub.QuadPart;
    }
};
#endif
