#include "../include/CpuMonitor.h"
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

void testCpuMonitorInitialization() {
    CpuMonitor monitor;
    float usage = monitor.getCpuUsage();
    assert(usage >= 0.0f && usage <= 100.0f);
    std::cout << "✓ CPU usage initialization test passed\n";
}

void testCpuFrequency() {
    CpuMonitor monitor;
    float freq = monitor.getCpuFreq();
    assert(freq > 0.0f);  // Frequency should be positive
    std::cout << "✓ CPU frequency test passed\n";
}

void testCpuInfoFormat() {
    CpuMonitor monitor;
    std::string info = monitor.getCpuInfo();
    assert(!info.empty());
    assert(info.find("CPU Usage:") != std::string::npos);
    assert(info.find("Core") != std::string::npos);
    std::cout << "✓ CPU info format test passed\n";
}

void testCpuUpdateFunction() {
    CpuMonitor monitor;
    bool updateResult = monitor.update();
    assert(updateResult == true);
    std::cout << "✓ CPU update function test passed\n";
}

void testCpuUsageOverTime() {
    CpuMonitor monitor;
    float usage1 = monitor.getCpuUsage();
    // Wait a bit and measure again
    std::this_thread::sleep_for(std::chrono::seconds(1));
    float usage2 = monitor.getCpuUsage();
    // Both readings should be valid percentages
    assert(usage1 >= 0.0f && usage1 <= 100.0f);
    assert(usage2 >= 0.0f && usage2 <= 100.0f);
    std::cout << "✓ CPU usage over time test passed\n";
}

int main() {
    try {
        std::cout << "Running CpuMonitor tests...\n";
        
        testCpuMonitorInitialization();
        testCpuFrequency();
        testCpuInfoFormat();
        testCpuUpdateFunction();
        testCpuUsageOverTime();
        
        std::cout << "\nAll tests passed successfully! ✓\n";
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
}