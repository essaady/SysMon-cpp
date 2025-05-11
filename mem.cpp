#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <unordered_map>

class MemoryMonitor {
public:
    MemoryMonitor();

    bool update();

    unsigned long getTotalMem() const noexcept;
    unsigned long getFreeMem() const noexcept;
    unsigned long getAvailableMem() const noexcept;
    float getUsedMemMB() const noexcept;
    float getMemUsagePercent() const noexcept;
    std::string toString() const;
    std::string toCSV() const;

private:
    std::unordered_map<std::string, unsigned long> memInfo_;
    
    bool parseProcMemInfo();
};

MemoryMonitor::MemoryMonitor() {
    update(); 
}

bool MemoryMonitor::update() {
    return parseProcMemInfo();
}

bool MemoryMonitor::parseProcMemInfo() {
    std::ifstream file("/proc/meminfo");
    if (!file.is_open()) return false;

    memInfo_.clear();
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        unsigned long value;
        std::string unit;

        iss >> key >> value >> unit;
        key.pop_back(); 
        
        if (unit == "kB") {
            memInfo_[key] = value;
        }
    }

    return !memInfo_.empty();
}

unsigned long MemoryMonitor::getTotalMem() const noexcept {
    return memInfo_.count("MemTotal") ? memInfo_.at("MemTotal") : 0;
}

unsigned long MemoryMonitor::getFreeMem() const noexcept {
    return memInfo_.count("MemFree") ? memInfo_.at("MemFree") : 0;
}

unsigned long MemoryMonitor::getAvailableMem() const noexcept {
    return memInfo_.count("MemAvailable") ? memInfo_.at("MemAvailable") : 0;
}

float MemoryMonitor::getUsedMemMB() const noexcept {
    return (getTotalMem() - getAvailableMem()) / 1024.0f;
}

float MemoryMonitor::getMemUsagePercent() const noexcept {
    if (getTotalMem() == 0) return 0.0f;
    return 100.0f * (1.0f - (getAvailableMem() / static_cast<float>(getTotalMem())));
}

std::string MemoryMonitor::toString() const {
    std::ostringstream oss;
    oss.precision(2);
    
    oss << "=== Mémoire RAM ===\n"
        << "Total: " << getTotalMem() / 1024 << " MB\n"
        << "Utilisé: " << getUsedMemMB() << " MB (" 
        << std::round(getMemUsagePercent()) << "%)\n"
        << "Disponible: " << getAvailableMem() / 1024 << " MB\n";

    return oss.str();
}

std::string MemoryMonitor::toCSV() const {
    std::ostringstream oss;
    oss << "type,total(MB),used(MB),usage(%)\n"
        << "RAM," << getTotalMem()/1024 << "," 
        << getUsedMemMB() << "," << getMemUsagePercent();
    return oss.str();
}
