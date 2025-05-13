#pragma once
#ifndef _SYS_MON
#define _SYS_MON

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstring>
#include <vector>
#include <unistd.h>

enum options
{
    _NLOG = 1000
};

class SysMon{
public:
    char *getTime();
    std::string getInfo(std::string _file_path);
    void log(std::ostream &out);
};

#endif