#include <iostream>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Drawer.h"
#include "Polygon.h"

int main() 
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SoftBody");
    Drawer drawer(window);
    sf::Clock Clock;
    World world({0.0f, 0.0f}, 10);

    auto body = std::make_shared<RigidBody>();
    body->Mass = 20.0f;
    body->InvMass = 1.0f / body->Mass;
    body->Position = {300.f, 300.f};
    body->I = 100.0f;
    body->InvI = 1.0f/body->I;
    // body->AngularVelocity = 0.0001f;
    body->SetSquare(100.0f);

    auto body2 = std::make_shared<RigidBody>();
    body2->Mass = 10.0f;
    body2->InvMass = 1.0f / body2->Mass;
    body2->Position = {400.f, 300.f};
    body2->I = 100.0f;
    body2->InvI = 1.0f/body2->I;
    body2->LinearVelocity = {-20.0f, 0.0f};
    body2->SetSquare(100.0f);
    body2->Rotate(1.0f);

    world.Add(body);
    world.Add(body2);
    
    while (window.isOpen())
    {
        sf::Time dt = Clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        world.Step(0.001f);
        window.clear();
        drawer.DrawBody(body);
        drawer.DrawBody(body2);
        drawer.DrawContacts(world.GetArbiters());
        window.display();
    }

    return 0;
}