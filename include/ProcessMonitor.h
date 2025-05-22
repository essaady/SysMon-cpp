#pragma once
#ifndef _PROC_H
#define _PROC_H

#include "../include/SysMon.h"
#include <filesystem>

class ProcMon : public SysMon{


public:
    void getProcess();

};

#endif