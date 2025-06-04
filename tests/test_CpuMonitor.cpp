#include "../include/CpuMonitor.h"
#include <iostream>
#include <cassert>
#include <thread>
#include <chrono>

void testCpuMonitorInitialization() {
    std::cout << "Testing CPU Monitor Initialization..." << std::endl;
    
    CpuMonitor cpuMonitor;
    
    // Test that CPU count is positive
    assert(cpuMonitor.getCpuCount() > 0);
    std::cout << "✓ CPU count is valid: " << cpuMonitor.getCpuCount() << std::endl;
    
    // Test initial CPU usage (should be >= 0 and <= 100)
    float usage = cpuMonitor.getCpuUsage();
    assert(usage >= 0.0f && usage <= 100.0f);
    std::cout << "✓ Initial CPU usage is valid: " << usage << "%" << std::endl;
    
    // Test CPU frequency (should be positive)
    float freq = cpuMonitor.getCpuFreq();
    assert(freq >= 0.0f);
    std::cout << "✓ CPU frequency is valid: " << freq << " MHz" << std::endl;
    
    std::cout << "CPU Monitor initialization tests passed!\n" << std::endl;
}

void testCpuMonitorUpdate() {
    std::cout << "Testing CPU Monitor Update..." << std::endl;
    
    CpuMonitor cpuMonitor;
    
    // Test update method
    bool updateResult = cpuMonitor.update();
    assert(updateResult == true);
    std::cout << "✓ CPU Monitor update successful" << std::endl;
    
    // Wait a bit and update again to test real monitoring
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    float usage1 = cpuMonitor.getCpuUsage();
    cpuMonitor.update();
    float usage2 = cpuMonitor.getCpuUsage();
    
    // Both readings should be valid
    assert(usage1 >= 0.0f && usage1 <= 100.0f);
    assert(usage2 >= 0.0f && usage2 <= 100.0f);
    std::cout << "✓ Multiple updates work correctly" << std::endl;
    std::cout << "  Usage 1: " << usage1 << "%" << std::endl;
    std::cout << "  Usage 2: " << usage2 << "%" << std::endl;
    
    std::cout << "CPU Monitor update tests passed!\n" << std::endl;
}

void testCpuPerCoreUsage() {
    std::cout << "Testing CPU Per-Core Usage..." << std::endl;
    
    CpuMonitor cpuMonitor;
    cpuMonitor.update();
    
    int coreCount = cpuMonitor.getCpuCount();
    
    // Test each core's usage
    for (int i = 0; i < coreCount; i++) {
        float coreUsage = cpuMonitor.getCpuUsagePerCore(i);
        assert(coreUsage >= 0.0f && coreUsage <= 100.0f);
        std::cout << "✓ Core " << i << " usage: " << coreUsage << "%" << std::endl;
    }
    
    // Test invalid core index
    float invalidUsage = cpuMonitor.getCpuUsagePerCore(-1);
    assert(invalidUsage == 0.0f);
    
    invalidUsage = cpuMonitor.getCpuUsagePerCore(coreCount + 10);
    assert(invalidUsage == 0.0f);
    std::cout << "✓ Invalid core indices handled correctly" << std::endl;
    
    std::cout << "CPU per-core usage tests passed!\n" << std::endl;
}

void testCpuInfo() {
    std::cout << "Testing CPU Info..." << std::endl;
    
    CpuMonitor cpuMonitor;
    cpuMonitor.update();
    
    std::string info = cpuMonitor.getCpuInfo();
    
    // Test that info string is not empty
    assert(!info.empty());
    std::cout << "✓ CPU info string is not empty" << std::endl;
    
    // Test that info contains expected keywords
    assert(info.find("CPU Usage") != std::string::npos);
    assert(info.find("CPU Frequency") != std::string::npos);
    assert(info.find("Number of CPUs") != std::string::npos);
    std::cout << "✓ CPU info contains expected content" << std::endl;
    
    std::cout << "CPU Info:\n" << info << std::endl;
    
    std::cout << "CPU info tests passed!\n" << std::endl;
}

void testCpuDataStructure() {
    std::cout << "Testing CPU Data Structure..." << std::endl;
    
    CpuMonitor cpuMonitor;
    cpuMonitor.update();
    
    auto cpuData = cpuMonitor.getCpuData();
    
    // Test CPU data structure
    assert(cpuData.nbrCPU > 0);
    assert(cpuData.usageCPU >= 0.0f && cpuData.usageCPU <= 100.0f);
    assert(cpuData.frequency >= 0.0f);
    assert(cpuData.usagePerCPU != nullptr);
    
    std::cout << "✓ CPU data structure is valid" << std::endl;
    std::cout << "  Number of CPUs: " << cpuData.nbrCPU << std::endl;
    std::cout << "  Overall usage: " << cpuData.usageCPU << "%" << std::endl;
    std::cout << "  Frequency: " << cpuData.frequency << " MHz" << std::endl;
    
    std::cout << "CPU data structure tests passed!\n" << std::endl;
}

void runStressTest() {
    std::cout << "Running CPU Monitor Stress Test..." << std::endl;
    
    CpuMonitor cpuMonitor;
    
    // Perform multiple rapid updates
    for (int i = 0; i < 100; i++) {
        bool result = cpuMonitor.update();
        assert(result == true);
        
        float usage = cpuMonitor.getCpuUsage();
        assert(usage >= 0.0f && usage <= 100.0f);
        
        if (i % 20 == 0) {
            std::cout << "✓ Stress test iteration " << i << " passed" << std::endl;
        }
    }
    
    std::cout << "CPU Monitor stress test passed!\n" << std::endl;
}

int main() {
    std::cout << "Running CPU Monitor Tests...\n" << std::endl;
    
    try {
        testCpuMonitorInitialization();
        testCpuMonitorUpdate();
        testCpuPerCoreUsage();
        testCpuInfo();
        testCpuDataStructure();
        runStressTest();
        
        std::cout << "🎉 All CPU Monitor tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "❌ Test failed with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "❌ Test failed with unknown exception" << std::endl;
        return 1;
    }
}