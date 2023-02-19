#pragma once

#include <SFML/Graphics.hpp>;

class Particle
{
public:
    Particle();
    Particle(float mass, sf::Vector2f pos, float radius);

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
