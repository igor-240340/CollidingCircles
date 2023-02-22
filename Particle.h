#pragma once

#include <SFML/Graphics.hpp>;

class Particle
{
public:
    Particle(sf::Vector2f pos, sf::Vector2f vel, float mass, float radius);
    Particle(float invMass);

    void SetPosition(sf::Vector2f pos);
    void ApplyForce(sf::Vector2f force);

public:
    sf::Vector2f pos;
    sf::Vector2f force;
    sf::Vector2f velocity;

    float invMass;
    float mass;
    float radius;
};
