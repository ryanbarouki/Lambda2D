#include "../headers/Drawer.h"
#include "../headers/MathsUtils.h"
#include "../headers/Polygon.h"
#include <memory>

void Drawer::DrawBody(std::shared_ptr<RigidBody> const& body)
{
    Mat22 R(body->Angle);
    Vec2 x = body->Position;
    // only works for Polygons rn
    auto& poly = dynamic_cast<Polygon&>(*body->Shape);
    std::vector<Vec2> vertices = poly.GetVertices();

    // feel like this shouldn't happen in the draw function 
    std::vector<Vec2> newVertices;
    sf::ConvexShape shape(vertices.size());
    int vNum = 0;
    for (auto const& v : vertices)
    {
        // vector from the centre to vertex
        auto relV = v - x;
        auto newV = x + R * relV; 
        shape.setPoint(vNum, {newV.x, newV.y});
        vNum++;
        newVertices.push_back(newV);
    }
    
    poly.SetVertices(newVertices);
    Window.draw(shape);
}