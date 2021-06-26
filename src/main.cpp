#include <iostream>
#include <SFML/Graphics.hpp>
#include "World.h"
#include "Drawer.h"
#include "Polygon.h"

void Demo1(World& world)
{
    // Bodies
    RigidBody body({500.0f, 300.0f}, 2.0f);
    body.SetRectangle(100.0f, 200.f);

    RigidBody body2({800.0f, 300.0f}, 8.0f);
    body2.LinearVelocity = {-200.0f, 0.0f};
    body2.AngularVelocity = 10.0f;
    body2.SetSquare(100.0f);
    body2.Rotate(1.0f);

    RigidBody body3({300.0f, 300.0f}, 2.0f);
    body3.SetRectangle(100.0f, 200.f);
    body3.LinearVelocity = {200.0f, 0.0f};

    RigidBody floor({500.0f, 800.0f});
    floor.SetRectangle(1000.0f, 10.0f, false);

    world.Add(body);
    world.Add(body2);
    world.Add(body3);
    world.Add(floor);
}

// stack
void Demo2(World& world)
{
    for (int i = 0; i < 15; ++i)
    {
        RigidBody body({500.0f, 100.0f + 60.0f * i}, 2.0f);
        body.LinearVelocity = {10.0f * i, 0.0f};
        body.SetSquare(40.0f);
        world.Add(body);
    } 

    RigidBody floor({500.0f, 970.0f});
    floor.SetRectangle(1000.0f, 10.0f, false);
    world.Add(floor);
}

constexpr int FRAME_RATE = 150;
constexpr bool CONST_FRAME_RATE = true;
int main() 
{
    sf::Clock Clock;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Rigid Body Physics");

    if (CONST_FRAME_RATE)
        window.setFramerateLimit(FRAME_RATE);

    World world({0.0f, 980.0f}, 10);
    Drawer drawer(window, world);

    Demo2(world);
    while (window.isOpen())
    {
        sf::Time dt = Clock.restart();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        world.Step(CONST_FRAME_RATE ? 1.0f/FRAME_RATE : dt.asSeconds());
        window.clear();
        drawer.DrawWorld();
        window.display();
    }

    return 0;
}