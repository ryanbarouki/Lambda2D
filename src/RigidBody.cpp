#include "../headers/RigidBody.h"
#include "Polygon.h"


RigidBody::RigidBody(Vec2 const& position, float mass) : Position(position), Mass(mass), Shape(std::make_unique<Polygon>(*this))
{
    InvMass = Mass != 0.0f ? 1.0f / Mass : 0.0f;
}

RigidBody::RigidBody(Vec2 const& position) : Position(position), Shape(std::make_unique<Polygon>(*this))
{
}

RigidBody::RigidBody(RigidBody const& body) 
: Position(body.Position),
LinearVelocity(body.LinearVelocity),
Mass(body.Mass),
InvMass(body.InvMass),
Angle(body.Angle), 
AngularVelocity(body.AngularVelocity),
I(body.I),
InvI(body.InvI),
Force(body.Force),
Torque(body.Torque),
Friction(body.Friction),
Aabb(body.Aabb)
{
    auto pPolygon = dynamic_cast<Polygon*>(body.Shape.get());
    if (pPolygon)
    {
        Shape = std::make_unique<Polygon>(pPolygon->GetVertices(), *this);
    }
}

AABB RigidBody::GetAABB() 
{
    // only works for polgons: need to figure this out for general shapes
    auto polygon = dynamic_cast<Polygon*>(Shape.get());
    if (!polygon)
    {
        std::cout << "polygon is null in GetAABB()\n";
        return {};
    }
    auto vertices = polygon->GetVertices();
    float x_min = std::numeric_limits<float>::max();
    float y_min = std::numeric_limits<float>::max();
    float x_max = std::numeric_limits<float>::min();
    float y_max = std::numeric_limits<float>::min();
    for (auto const& v : vertices)
    {
        if (v.x < x_min)
            x_min = v.x;
        if (v.x > x_max)
            x_max = v.x;
        if (v.y < y_min)
            y_min = v.y;
        if (v.y > y_max)
            y_max = v.y;
    }

    return {Position + Vec2{x_min, y_min}, Position + Vec2{x_max, y_max}};
}

void RigidBody::SetSquare(float width)
{
    auto pPolygon = dynamic_cast<Polygon*>(Shape.get());
    if (pPolygon)
    {
        pPolygon->SetSquare(width);
    }
    // moment of inertia of a square about centre
    I = Mass * width * width / 6;
    InvI = 1.0f / I;
}

void RigidBody::SetRectangle(float width, float height, bool phys /* = true */)
{
    auto pPolygon = dynamic_cast<Polygon*>(Shape.get());
    if (pPolygon)
    {
        pPolygon->SetRectangle(width, height);
    }
    if (phys)
    {
        // moment of inertia of a square about centre
        I = Mass * (height * height + width * width)/ 12;
        InvI = 1.0f / I;
    }
}

void RigidBody::Rotate(float dA)
{
    Angle += dA;
    Mat22 R(dA);
    if (auto pPolygon = dynamic_cast<Polygon*>(Shape.get()))
    {
        std::vector<Vec2> newVertices;
        for (auto const& v : pPolygon->GetVertices())
        {
            newVertices.push_back(R * v);     
        }

        pPolygon->SetVertices(newVertices);
    }
}