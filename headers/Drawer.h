#pragma once
#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include <memory>

class Drawer
{
public:

    Drawer(sf::RenderWindow& window) : Window(window) {}
    void DrawBody(std::shared_ptr<RigidBody> const& body);
    void Clear()
    {
        Window.clear();
    }
    void Display()
    {
        Window.display();
    }
private:

    sf::RenderWindow& Window;
};
