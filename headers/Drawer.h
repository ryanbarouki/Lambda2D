#pragma once
#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include "World.h"
#include <memory>

class Drawer
{
public:

    Drawer(sf::RenderWindow& window) : Window(window) {}
    void DrawBody(std::shared_ptr<RigidBody> const& body);
    void DrawContacts(std::map<ArbiterKey, Arbiter> const& arbiters);
private:

    sf::RenderWindow& Window;
};
