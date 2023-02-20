#pragma once

#include <SFML/Graphics.hpp>

#include "PhysicalWorld.h"

class App
{
public:
    App();
    ~App();

    void Run();

private:
    static const sf::Time FIXED_DELTA_TIME;
    static const int CIRCLE_COUNT = 10;

    PhysicalWorld world;

    sf::RenderWindow* window;
    sf::CircleShape* circleShape;

    void HandleInput();
    void FixedUpdate(sf::Time dt);
    void Render();

    void FillWorldWithCircles();
};
