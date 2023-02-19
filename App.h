#pragma once

#include <SFML/Graphics.hpp>

#include "PhysicalWorld.h"

class App
{
public:
    App();
    void Run();
    static const int RADIUS;

private:
    static const sf::Time SECS_PER_FRAME;

    sf::Font mFont;
    sf::Text	 mStatisticsText;
    sf::Time	 mStatisticsUpdateTime;

    std::size_t mStatisticsNumFrames;

    void UpdateStat(sf::Time elapsedTime);
    void HandleEvents();
    void FixedUpdate(sf::Time);
    void Display();

    sf::RenderWindow window;

    PhysicalWorld world;

    sf::CircleShape shape;
};
