#ifdef _WIN32
#include <windows.h>
#include <psapi.h>  // مكتبة للحصول على معلومات عن العمليات
#else
#include <unistd.h>
#endif
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
using namespace std;

ProcessMonitor::ProcessMonitor() {
    lastTotalCpuTime = getTotalCpuTime();
}

void ProcessMonitor::update() {
    readProcesses();
}

const vector<ProcessInfo>& ProcessMonitor::getProcesses() const {
    return processes;
}

unsigned long long ProcessMonitor::getTotalCpuTime() const {
    ifstream file("/proc/stat");
    string line;
    unsigned long long total = 0;

    if (file.is_open()) {
        getline(file, line);
        istringstream iss(line);
        string cpu;
        iss >> cpu;

        unsigned long long time;
        while (iss >> time) {
            total += time;
        }
    }

    return total;
}

void ProcessMonitor::readProcesses() {
    processes.clear();

    // معالجة للمعلومات حسب البيئة (Linux أو Windows)
    #ifdef _WIN32
        // الحصول على قائمة العمليات في Windows
        DWORD aProcesses[1024], cbNeeded, cProcesses;
        if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) {
            return;
        }
        cProcesses = cbNeeded / sizeof(DWORD);

        for (unsigned int i = 0; i < cProcesses; i++) {
            DWORD pid = aProcesses[i];
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
            if (hProcess == NULL) {
                continue;
            }

            // جلب اسم العملية
            TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
            GetModuleBaseName(hProcess, NULL, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));
            
            // إضافة العملية إلى القائمة
            processes.push_back({pid, szProcessName, 0.0, 0.0});  // قم بتحديد القيم الأخرى بناءً على احتياجك.
            CloseHandle(hProcess);
        }
    #else
        // كود Linux الحالي
        DIR* dir = opendir("/proc");
        struct dirent* entry;

        unsigned long long currentTotalCpuTime = getTotalCpuTime();
        unsigned long long totalDelta = currentTotalCpuTime - lastTotalCpuTime;
        lastTotalCpuTime = currentTotalCpuTime;

        while ((entry = readdir(dir)) != nullptr) {
            if (!isdigit(entry->d_name[0])) continue;

            int pid = atoi(entry->d_name);
            string statPath = "/proc/" + string(entry->d_name) + "/stat";
            ifstream statFile(statPath);

            if (!statFile) continue;

            string line;
            getline(statFile, line);
            istringstream iss(line);

            string comm;
            char state;
            unsigned long utime, stime;
            unsigned long vsize;
            long rss;

            int skip;
            iss >> skip >> comm >> state;

            // On avance jusqu’à utime (14ème champ)
            for (int i = 0; i < 11; ++i) iss >> skip;
            iss >> utime >> stime;

            // On avance jusqu’à vsize et rss
            for (int i = 0; i < 6; ++i) iss >> skip;
            iss >> vsize >> rss;

            // Nom du processus
            string name = comm.substr(1, comm.size() - 2); // enlever les parenthèses

            // Calcul du CPU utilisé (utime + stime)
            unsigned long long procTime = utime + stime;

            double cpuUsage = (totalDelta > 0) ? 100.0 * procTime / totalDelta : 0.0;

            // Mémoire en Mo
            long pageSizeKB = sysconf(_SC_PAGESIZE) / 1024;
            double memUsageMB = rss * pageSizeKB / 1024.0;

            processes.push_back({pid, name, cpuUsage, memUsageMB});
        }

        closedir(dir);
    #endif
}
