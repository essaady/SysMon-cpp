/*
Create a small system that monitors CPU usage by reading data from the Linux system file /proc/stat, then calculate and show overall and per-core usage.
!! Key Steps to Follow:
Understand the /proc/stat file:
This file contains CPU activity data.
Each line starting with cpu gives cumulative time values in different states: user, system, idle, etc.
Design a structure to hold CPU data:
Create a structure/class that stores the values you extract (e.g., user time, system time, idle time, etc.)
Add helper functions to compute total time and idle time.
Create a class to manage monitoring:
Add a method to parse the /proc/stat file and store data for each core.
Keep both current and previous readings, so you can compare between them.
Compute usage percentage:
Use the difference between current and previous values to calculate CPU usage per core and total.
Add public methods like:
update() → To refresh data from the system.
getOverallCpuUsage() → To return the global CPU usage.
getPerCoreCpuUsage() → To return usage per core.
(Optional) exportData() → To export stats to a file.
Focus on clean structure:
Use C++ features like std::unordered_map, std::vector, and classes to keep everything organized.
Tips:
Keep your code modular and clear.
Start small (only total CPU usage), then add per-core logic.
Comment your code to explain your logic.
*/
