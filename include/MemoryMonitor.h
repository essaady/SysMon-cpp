#pragma once
#ifndef _MEM_MON_H
#define _MEM_MON_H

#include "./SysMon.h"

class MemInfo : public SysMon{
public:
    size_t memUsage(int log);
};

#endif