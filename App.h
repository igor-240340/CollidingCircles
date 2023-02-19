#pragma once

#include <SFML/Graphics.hpp>

#include "PhysicalWorld.h"

class App
{
public:
    App();
    void Run();

private:
    static const sf::Time TimePerFrame;

    void HandleEvents();
    void FixedUpdate(sf::Time);
    void Display();

    sf::RenderWindow window;

    PhysicalWorld world;

    sf::CircleShape shape;
};
