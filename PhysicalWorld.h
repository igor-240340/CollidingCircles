#pragma once

#include <list>

#include "Particle.h";

class PhysicalWorld
{
public:
    PhysicalWorld(float width, float height);

    void AddParticle(Particle& particle);
    const std::list<Particle>& Particles();
    void Step(float dt);

private:
    static const float DRAG_COEFF;  // Сила сопротивления среды (для постепенного замедления объектов).
    static const float RESTITUTION; // Коэффициент восстановления, меняется в пределах [0..1].

    float width;
    float height;                   // Размеры мира (для определения границ).

    std::list<Particle> particles;

    void HandleCollision();
    void ResolveVelocity(Particle& pA, Particle& pB, sf::Vector2f hitNormal);
    void ResolvePenetration(Particle& pA, Particle& pB, float penetration, sf::Vector2f hitNormal);
    void HandleCollisionWithBorders(Particle& pA);

    float Dot(sf::Vector2f a, sf::Vector2f b);
    float SqrLen(sf::Vector2f v);
};
