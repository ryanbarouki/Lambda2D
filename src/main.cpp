#include <iostream>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Drawer.h"
#include "Polygon.h"

static bool DEBUG = false;
int main() 
{
    sf::Clock Clock;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SoftBody");
    World world({0.0f, 500.0f}, 10);
    Drawer drawer(window, world);

    // Bodies 
    RigidBody body({500.0f, 300.0f}, 2.0f);
    body.SetRectangle(100.0f, 200.f);

    RigidBody body2({800.0f, 300.0f}, 8.0f);
    body2.LinearVelocity = {-200.0f, 0.0f};
    body2.AngularVelocity = 10.0f;
    body2.SetSquare(100.0f);
    body2.Rotate(1.0f);

    RigidBody floor({500.0f, 800.0f});
    floor.SetRectangle(1000.0f, 10.0f, false);

    world.Add(body);
    world.Add(body2);
    world.Add(floor);
    
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
        drawer.DrawWorld();
        window.display();
    }

    return 0;
}