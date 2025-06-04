cmake_minimum_required( VERSION 3.22 )

project( Systeme_Monitor )

set( CMAKE_CXX_STANDARD 20)

add_library(cpu STATIC src/CpuMonitor.cpp src/MemoryMonitor.cpp src/SysMon.cpp src/ProcessMonitor.cpp)
target_include_directories(cpu PRIVATE ./include)


add_executable(SysMon ./src/main.cpp)
target_link_libraries(SysMon PUBLIC cpu)

