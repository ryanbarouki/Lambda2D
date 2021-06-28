#include "../headers/Drawer.h"
#include "../headers/MathsUtils.h"
#include "../headers/Polygon.h"
#include <memory>
#include <iostream>
#include <sstream>

void Drawer::DrawWorld()
{
    for (auto const& body : World.GetBodies())
    {
        DrawBody(body);
    }
    DrawContacts(World.GetArbiters());
}

void Drawer::DrawBody(std::shared_ptr<RigidBody> const& body)
{
    // Mat22 R(body->Angle);
    if (!body)
    {
        std::cout << "Body is null\n";
        return;
    }
    Vec2 x = body->Position;
    // only works for Polygons rn
    auto poly = dynamic_cast<Polygon*>(body->Shape.get());

    if (!poly)
    {
        std::cout << "polygon is null in DrawBody()\n";
        return;
    }

    std::vector<Vec2> vertices = poly->GetVertices();

    // feel like this shouldn't happen in the draw function 
    sf::ConvexShape shape(vertices.size());
    int vNum = 0;
    for (auto const& v : vertices)
    {
        // vector from the centre to vertex
        // auto newV = R * v; 
        shape.setPoint(vNum++, {x.x + v.x, x.y + v.y});
    }
    
    Window.draw(shape);
}

void Drawer::DrawContacts(std::map<ArbiterKey, Arbiter> const& arbiters)
{
    for (auto const& [arbKey, arb] : arbiters)
    {
        for (auto const& c : arb.GetContacts())
        {
            sf::CircleShape point(2.0f);
            // sf::Text coords;
            // std::stringstream coordStr;
            // coordStr << "(" << c.point.x << ", " << c.point.y << ")";
            // sf::Font font;
            // font.loadFromFile("..\\resources\\arial.ttf");
            // coords.setFont(font);
            // coords.setString(coordStr.str());
            // coords.setFillColor(sf::Color::Red);
            point.setPosition({c.point.x, c.point.y});
            point.setFillColor(sf::Color::Red);
            // Window.draw(coords);
            Window.draw(point);
        }
    }
}