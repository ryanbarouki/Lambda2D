#include <iostream>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Drawer.h"
#include "Polygon.h"

static bool DEBUG = false;
int main() 
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SoftBody");
    Drawer drawer(window);
    sf::Clock Clock;
    World world({0.0f, 0.0f}, 10);

    auto body = std::make_shared<RigidBody>();
    body->Mass = 2.0f;
    body->InvMass = 1.0f / body->Mass;
    body->Position = {500.f, 300.f};
    body->SetRectangle(100.0f, 200.f);

    auto body2 = std::make_shared<RigidBody>();
    body2->Mass = 2.0f;
    body2->InvMass = 1.0f / body2->Mass;
    body2->Position = {800.f, 300.f};
    body2->LinearVelocity = {-200.0f, 0.0f};
    body2->AngularVelocity = 3.f;
    body2->SetSquare(100.0f);
    body2->Rotate(1.0f);

    auto floor = std::make_shared<RigidBody>();
    floor->Position = {500.0f, 900.0f};
    floor->SetRectangle(1000.0f, 10.0f, false);

    world.Add(std::move(body));
    world.Add(std::move(body2));
    world.Add(std::move(floor));
    
    while (window.isOpen())
    {
        sf::Time dt = Clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        world.Step(DEBUG ? 0.001f : dt.asSeconds());
        window.clear();
        drawer.DrawBody(body);
        drawer.DrawBody(body2);
        drawer.DrawBody(floor);
        drawer.DrawContacts(world.GetArbiters());
        window.display();
    }

    return 0;
}