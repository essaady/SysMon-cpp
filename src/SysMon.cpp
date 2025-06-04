#include "../include/SysMon.h"
#include <unistd.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

SysMon::SysMon(int updateInterval, bool fullLog): CpuMonitor(), MemoryMonitor(), ProcessMonitor(){
    this->updateInterval=updateInterval;
    this->fullLog=fullLog;
}

SysMon::~SysMon(){}


int SysMon::run(int limit){
    int i = 0;
    while (i < limit)
    {
        usleep(updateInterval);
        if(!fullLog){
            system("clear");
        }
        update();
        i++;
    }
    return 0;
}

bool SysMon::update() {
    bool success = true;
    success &= CpuMonitor::update();
    success &= ProcessMonitor::update();
    success &= MemoryMonitor::update();
    return success;
}

string SysMon::exportAsText() {
    stringstream ss;
    ss << "=== System Monitor Report ===" << endl;
    ss << "Time: " << getTime() << endl << endl;
    
    // CPU Information
    ss << "CPU Usage: " << getCpuUsage() << "%" << endl;
    ss << "CPU Frequency: " << getCpuFreq() << " MHz" << endl;
    ss << getCpuInfo() << endl;
    
    // Memory Information
    ss << "Memory Usage: " << getMemoryUsagePercentage() << "%" << endl;
    ss << "Total Memory: " << getTotalMemory() << " MB" << endl;
    ss << "Free Memory: " << getFreeMemory() << " MB" << endl;
    ss << "Swap Usage: " << getSwapUsagePercentage() << "%" << endl;
    
    // Process Information
    ss << getProcessInfo() << endl;
    
    return ss.str();
}

string SysMon::exportAsCSV() {
    stringstream ss;
    ss << "Timestamp,CPU Usage,Memory Usage,Process Count" << endl;
    ss << getTime() << "," 
       << getCpuUsage() << "," 
       << getMemoryUsagePercentage() << "," 
       << nbrProcess << endl;
    return ss.str();
}

//function to get the current time for loggin.
string SysMon::getTime()
{
    time_t timestamp;
    time(&timestamp);
    char* time = ctime(&timestamp);

    if (time[strlen(time) - 1] == '\n')
        time[strlen(time) - 1] = '\0';
    return time;
}

// //Fetching required files '/proc/stat/' and '/proc/meminfo'.
string SysMon::getInfo(string _file_path)
{
    string content;
    stringstream content_stream;

    fstream info(_file_path, ios::in);
    if (!info)
    {
        cerr << "There was an error opening the file" << endl;
        return " ";
    }
    content_stream << info.rdbuf();
    content = content_stream.str();
    info.close();
    return content;
}

void SysMon::log(ostream &out)
{
    fstream logFile("log.txt", ios::app);
    logFile << out.rdbuf();
    logFile.close();
}

std::vector<std::string> getVector(std::istringstream &iss)
{
    int i = 0;
    std::vector<std::string> info;
    std::string content;
    while (iss >> content && i < 13)
    {
        info.push_back(content);
        i++;
    }

    return info;
}

bool isNumber(std::string &string)
{
    bool isNumber = true;
    for (auto str : string)
    {
        if (!std::isdigit(str))
        {
            isNumber = false;
            break;
        }
    }
    return isNumber;
}