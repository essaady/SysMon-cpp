cmake_minimum_required(VERSION 3.10)
project(SysMon-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add all source files
set(SOURCES
    src/main.cpp
    src/CpuMonitor.cpp
    src/MemoryMonitor.cpp
    src/ProcessMonitor.cpp
)

# Create executable
add_executable(sysmon ${SOURCES})