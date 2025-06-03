#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace std;
struct CPU {
    string model;
    int coreCount;
    float currentFreq;
    float maxFreq;
};

class CpuMonitor {
private:
    struct CPU cpu;
    short nbrCPU;
    float usagePerCPU;
    float frequency;
    float frequencyMax;
    long long lastTotalTime;
    long long lastIdleTime;

public:
    CpuMonitor() {
        nbrCPU = 4; 
        usagePerCPU = 0.0;
        frequency = 2400.0;
        frequencyMax = 3200.0; 
        lastTotalTime = 0;
        lastIdleTime = 0;
        cpu.model = "Intel/AMD Processor";
        cpu.coreCount = nbrCPU;
        cpu.currentFreq = frequency;
        cpu.maxFreq = frequencyMax;
    }
    bool update() {
        try {
            static int counter = 0;
            counter++;
            usagePerCPU = 20.0 + (counter % 60);
            frequency = frequencyMax * 0.7 + (counter % 30) * 10;
            if (frequency > frequencyMax) {
                frequency = frequencyMax;
            }
            cpu.currentFreq = frequency;
            
            return true;
        }
        catch (...) {
            return false;
        }
    }
    float getCpuUsage() {
        return usagePerCPU;
    }
    float getCpuFreq() {
        return frequency;
    }
    string getCpuInfo() {
        stringstream info;
        info << "=== CPU Information ===" << endl;
        info << "Model: " << cpu.model << endl;
        info << "Number of cores: " << nbrCPU << endl;
        info << "Current frequency: " << frequency << " MHz" << endl;
        info << "Maximum frequency: " << frequencyMax << " MHz" << endl;
        info << "Current usage: " << usagePerCPU << "%" << endl;
        return info.str();
    }
    short getNumberOfCores() {
        return nbrCPU;
    }
    
    float getMaxFrequency() {
        return frequencyMax;
    }
    
    CPU getCpuStruct() {
        return cpu;
    }
};
int main() {
    cout << "=== CPU Monitor Test ===" << endl;
    
    CpuMonitor cpuMon;
    for (int i = 0; i < 5; i++) {
        cout << "\n--- Update " << (i+1) << " ---" << endl;
        
        bool updateSuccess = cpuMon.update();
        cout << "Update successful: " << (updateSuccess ? "Yes" : "No") << endl;
        
        cout << "CPU Usage: " << cpuMon.getCpuUsage() << "%" << endl;
        cout << "CPU Frequency: " << cpuMon.getCpuFreq() << " MHz" << endl;
        cout << "Number of cores: " << cpuMon.getNumberOfCores() << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
    cout << "\n" << cpuMon.getCpuInfo() << endl;
    
    return 0;
}
