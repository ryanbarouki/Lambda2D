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
    World world({0.0f, 1.0f}, 10);
    Polygon square;
    square.SetSquare({100,100}, 100);
    auto body = std::make_shared<RigidBody>(std::make_unique<Polygon>(square));
    
    float vel = 0.f;
    float g = 1000.f; // pixels / sec^2
    while (window.isOpen())
    {
        sf::Time dt = Clock.restart();
        sf::Event event;
        world.Step(dt.asSeconds());
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        drawer.DrawBody(body);
        window.display();
    }

    return 0;
}