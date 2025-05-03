#include "../include/SysMon.h"

//function to get the current time for loggin.
char *SysMon::getTime()
{
    time_t timestamp;
    time(&timestamp);
    char *time = ctime(&timestamp);

    if (time[strlen(time) - 1] == '\n')
        time[strlen(time) - 1] = '\0';
    return time;
}

//Fetching required files '/proc/stat/' and '/proc/meminfo'.
std::string SysMon::getInfo(std::string _file_path)
{
    std::string content;
    std::stringstream content_stream;

    std::fstream info(_file_path, std::ios::in);
    if (!info)
    {
        std::cerr << "There was an error opening the file" << std::endl;
        exit(-1);
    }
    content_stream << info.rdbuf();
    content = content_stream.str();
    return content;
}

void SysMon::log(std::ostream &out)
{
    std::fstream logFile("log.txt", std::ios::app);
    logFile << out.rdbuf();
    logFile.close();
}