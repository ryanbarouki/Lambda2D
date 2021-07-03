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
    body3.AngularVelocity = 6.0f;

    RigidBody floor({500.0f, 800.0f});
    floor.SetRectangle(1500.0f, 10.0f, false);

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
        RigidBody body({500.0f, 50.0f * i}, 2.0f);
        body.LinearVelocity = {3.0f * i, 0.0f};
        body.SetSquare(40.0f);
        world.Add(body);
    } 
    RigidBody floor({500.0f, 800.0f});
    floor.SetRectangle(1500.0f, 10.0f, false);
    world.Add(floor);
}

// shower
void Demo3(World& world)
{
    int numInStack = 40;
    for (int i = 0; i < numInStack; ++i)
    {
        RigidBody body({500.0f, 25.0f * i}, 2.0f);
        body.LinearVelocity = {10.0f * i * powf(-1, i), 100.0f};
        body.SetSquare(20.0f);
        world.Add(body);
    } 
    for (int i = 0; i < numInStack; ++i)
    {
        RigidBody body({200.0f, 25.0f * i}, 2.0f);
        body.LinearVelocity = {10.0f * i * powf(-1, i), -80.0f};
        body.SetSquare(20.0f);
        world.Add(body);
    } 
    for (int i = 0; i < numInStack; ++i)
    {
        RigidBody body({700.0f, 25.0f * i}, 2.0f);
        body.LinearVelocity = {10.0f * i * powf(-1, i), -60.0f};
        body.SetSquare(20.0f);
        world.Add(body);
    }
    // Note that static objects can't overlap or they will break
    RigidBody floor({500.0f, 900.0f});
    floor.SetRectangle(850.0f, 10.0f, false);
    RigidBody leftWall({0.0f, 500.0f});
    leftWall.SetRectangle(100.0f, 1000.0f, false);
    RigidBody rightWall({1000.0f, 500.0f});
    rightWall.SetRectangle(100.0f, 1000.0f, false);
    world.Add(floor);
    world.Add(leftWall);
    world.Add(rightWall);
}

void Demo4(World& world)
{
    int numBlocks = 300;
    int numPerRow = 30;
    float y = 0.0f;
    for (int i = 0; i < numBlocks; ++i)
    {
        y = (i % numPerRow) == 0 ? y + 25.0f : y;
        RigidBody body({25.0f * (i % numPerRow) + 110.0f, y}, 2.0f);
        // body.LinearVelocity = {-300.0f * powf(-1, i), 200.0f};
        body.SetSquare(20.0f);
        world.Add(body);
    }
    RigidBody floor({500.0f, 900.0f});
    floor.SetRectangle(850.0f, 10.0f, false);
    RigidBody leftWall({0.0f, 500.0f});
    leftWall.SetRectangle(100.0f, 1000.0f, false);
    RigidBody rightWall({1000.0f, 500.0f});
    rightWall.SetRectangle(100.0f, 1000.0f, false);
    world.Add(floor);
    world.Add(leftWall);
    world.Add(rightWall);
}

constexpr int FRAME_RATE = 150;
constexpr bool CONST_FRAME_RATE = true;
int main() 
{
    sf::Clock Clock;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Lambda2D");

    if (CONST_FRAME_RATE)
        window.setFramerateLimit(FRAME_RATE);

    World world({0.0f, 981.0f}, 10);
    Drawer drawer(window, world);

    Demo3(world);
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