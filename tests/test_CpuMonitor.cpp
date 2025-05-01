#include <iostream>
#include <chrono>
#include <thread>
#include "../include/CpuMonitor.h"

// Test function for the CpuMonitor class (Windows version)
void test_cpu_monitor() {
    std::cout << "Starting Windows CPU Monitor Test...\n";
    
    // Create a CPU monitor instance
    CpuMonitor monitor;
    
    // Test getting CPU cores count
    std::cout << "Number of CPU cores: " << monitor.getNumCores() << "\n\n";
    
    // Test update and get initial usage
    monitor.update();
    std::cout << "Initial CPU Usage: " << monitor.getUsage() << "%\n\n";
    
    // Simulate some CPU load for testing
    std::cout << "Simulating CPU load for 3 seconds...\n";
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3)) {
        // Busy wait to create CPU load
        volatile double dummy = 0.0;
        for (int i = 0; i < 10000000; i++) {
            dummy += i * 1.1;
        }
    }
    
    // Wait a moment before taking the next measurement
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    // Update and get usage after load
    monitor.update();
    std::cout << "CPU Usage after load: " << monitor.getUsage() << "%\n\n";
    
    // Test getAllUsages function
    auto allUsages = monitor.getAllUsages();
    std::cout << "All CPU Usages:\n";
    for (const auto& pair : allUsages) {
        std::cout << pair.first << ": " << pair.second << "%\n";
    }
    std::cout << "\n";
    
    // Test export functions
    std::cout << "Export to Text:\n";
    std::cout << monitor.exportToText() << "\n";
    
    std::cout << "Export to CSV:\n";
    std::cout << monitor.exportToCsv() << "\n";
    
    std::cout << "CPU Monitor Test Completed.\n";
}

// Main function for the test
int main() {
    test_cpu_monitor();
    // Keep the console window open until user presses a key
    std::cout << "\nPress any key to exit...";
    std::cin.get();
    return 0;
}