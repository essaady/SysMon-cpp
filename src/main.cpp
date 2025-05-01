#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <windows.h>
#include <vector>
#include "../include/CpuMonitor.h"

// Function to set console cursor position
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Hide console cursor
void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

// Set console color
void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Function to draw CPU usage bar
void drawCpuBar(int startX, int startY, double percentage, int width, const std::string& label) {
    // Calculate filled width
    int filledWidth = static_cast<int>(percentage / 100.0 * width);
    
    // Draw label
    setCursorPosition(startX, startY);
    std::cout << std::setw(7) << std::left << label;
    
    // Draw percentage
    std::cout << std::setw(6) << std::right << std::fixed << std::setprecision(1) << percentage << "% ";
    
    // Draw bar
    std::cout << "[";
    
    // Determine color based on usage
    if (percentage < 60.0) {
        setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY); // Bright green
    } else if (percentage < 85.0) {
        setConsoleColor(FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY); // Bright yellow
    } else {
        setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY); // Bright red
    }
    
    // Draw filled part
    for (int i = 0; i < filledWidth; i++) {
        std::cout << "█";
    }
    
    // Reset color and draw empty part
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    for (int i = filledWidth; i < width; i++) {
        std::cout << " ";
    }
    
    std::cout << "]";
}

int main() {
    // Set console title
    SetConsoleTitle(TEXT("SysMon-cpp CPU Monitor"));
    
    // Set up console
    system("cls");
    hideCursor();
    
    // Set console color to white
    setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    
    // Create header
    std::cout << "╔════════════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                      SysMon-cpp CPU Monitor                        ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════════════╝" << std::endl;
    std::cout << std::endl;
    
    // Create CPU monitor instance
    CpuMonitor cpuMonitor;
    
    // Get number of cores
    int numCores = cpuMonitor.getNumCores();
    std::cout << "Detected " << numCores << " CPU cores" << std::endl;
    std::cout << std::endl;
    std::cout << "Press Ctrl+C to exit..." << std::endl;
    std::cout << std::endl;
    
    // Line where stats will start
    int startLine = 9;
    
    // Monitor and display CPU usage
    try {
        while (true) {
            // Update CPU stats
            cpuMonitor.update();
            
            // Get usage data
            double totalUsage = cpuMonitor.getUsage("cpu");
            
            // Draw total CPU usage
            drawCpuBar(2, startLine, totalUsage, 50, "Total");
            
            // Draw individual core usages
            for (int i = 0; i < numCores; i++) {
                std::string cpuId = "cpu" + std::to_string(i);
                double coreUsage = cpuMonitor.getUsage(cpuId);
                drawCpuBar(2, startLine + i + 1, coreUsage, 50, "Core " + std::to_string(i));
            }
            
            // Wait before next update
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    } catch (const std::exception& e) {
        setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
        std::cerr << "Error: " << e.what() << std::endl;
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return 1;
    }
    
    return 0;
}