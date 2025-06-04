#include "SysMon.h"
#include <sstream>
#include <iomanip>

std::string SysMon::exportAsJSON(CpuMonitor cpuMon, MemoryMonitor memMon, ProcessMonitor procMon) {
    std::ostringstream json;
    json << "{\n";

    json << "  \"cpu\": {\n";
    json << "    \"usage\": " << std::fixed << std::setprecision(2) << cpuMon.getCpuUsage() << "\n";
    json << "  },\n";

    json << "  \"memory\": {\n";
    json << "    \"used_percent\": " << memMon.getMemoryUsagePercentage() << "\n";
    json << "  },\n";

    json << "  \"processes\": [\n";
    for (int i = 0; i < procMon.nbrProcess; ++i) {
        auto p = procMon.getProcess(i);
        json << "    {\n";
        json << "      \"user\": \"" << p.user << "\",\n";
        json << "      \"cpu\": " << p.cpu << ",\n";
        json << "      \"memory\": " << p.memory << ",\n";
        json << "      \"path\": \"" << p.pathName << "\"\n";
        json << "    }";
        if (i < procMon.nbrProcess - 1) json << ",";
        json << "\n";
    }
    json << "  ]\n";
    json << "}";

    return json.str();
}
