#pragma once
#include <SFML/Graphics.hpp>
#include "RigidBody.h"
#include "World.h"
#include <memory>

class Drawer
{
public:

    Drawer(sf::RenderWindow& window, World& world) : Window(window), World(world) {}
    void DrawWorld();

private:

    void DrawBody(std::shared_ptr<RigidBody> const& body);
    void DrawContacts(std::map<ArbiterKey, Arbiter> const& arbiters);
    sf::RenderWindow& Window;
    World& World;
};
