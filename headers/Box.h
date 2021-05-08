#pragma once
#include "IShape2.h"

class Box : public IShape2 
{
private:
    float Mass;
    float Width;
    float Height;
    float MomentOfIntertia;

public:
    Box() = default;
    Box(float mass, float width, float height);
    float GetMass() { return Mass; }
    float GetMomentOfIntertia() { return MomentOfIntertia; }
};