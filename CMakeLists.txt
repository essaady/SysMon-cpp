cmake_minimum_required(VERSION 3.22)

project(Systeme_Monitor VERSION 1.0)

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Platform specific settings
if(WIN32)
    # Windows specific settings
    add_definitions(-D_WIN32_WINNT=0x0601)
    if(MSVC)
        add_compile_options(/W4 /EHsc)
    endif()
else()
    # Linux specific settings
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_definitions(-DLINUX)
endif()

# Library target
add_library(cpu STATIC 
    src/CpuMonitor.cpp 
    src/MemoryMonitor.cpp 
    src/SysMon.cpp 
    src/ProcessMonitor.cpp
)
target_include_directories(cpu PUBLIC ./include)

# Main executable
add_executable(SysMon ./src/main.cpp)
target_link_libraries(SysMon PUBLIC cpu)

# Enable testing
enable_testing()

# Test executable
add_executable(test_cpu_monitor tests/test_CpuMonitor.cpp)
target_link_libraries(test_cpu_monitor PRIVATE cpu)
add_test(NAME CpuMonitorTests COMMAND test_cpu_monitor)

