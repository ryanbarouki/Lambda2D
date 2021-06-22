#pragma once
#include "AABB.h"

class IShape2
{
public:
    IShape2(){}
    virtual ~IShape2() {}
    // common pure virtual methods to go here
    virtual AABB GetAABB() const = 0;
};