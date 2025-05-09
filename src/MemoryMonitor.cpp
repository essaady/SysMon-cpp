#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/resource.h>
#endif

void printMemoryUsage() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS memCounter;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memCounter, sizeof(memCounter))) {
        std::cout << "Memory used (working set size): " << memCounter.WorkingSetSize / 1024 << " KB" << std::endl;
    } else {
        std::cerr << "Failed to retrieve memory info." << std::endl;
    }
#else
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        std::cout << "Memory used (max resident set size): " << usage.ru_maxrss << " KB" << std::endl;
    } else {
        std::cerr << "Failed to retrieve memory info." << std::endl;
    }
#endif
}

int main() {
    std::cout << "Starting Memory Monitor..." << std::endl;

    // Simulate some memory usage
    int* arr = new int[1000000]; 
    for (int i = 0; i < 1000000; ++i) arr[i] = i;

    printMemoryUsage();
    delete[] arr;
    return 0;
}

