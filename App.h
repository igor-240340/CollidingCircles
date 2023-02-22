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
    static const int WIDTH = 1024;
    static const int HEIGHT = 768;
    static const int CIRCLE_COUNT = 10;

    static const sf::Color BACKGROUND_COLOR;
    static const sf::Color CIRCLE_COLOR;
    static const sf::Time FIXED_DELTA_TIME;

    sf::RenderWindow* window;
    sf::CircleShape* circleShape;

    PhysicalWorld world;

    void HandleInput();
    void FixedUpdate(sf::Time dt);
    void Render();

    void FillWorldWithCircles();
};
