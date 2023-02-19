#pragma once

#include <list>

#include "Particle.h";

class PhysicalWorld
{
public:
    PhysicalWorld();

    void AddParticle(Particle&);
    void Step(float dt);

    std::list<Particle> particles;

private:
    bool isPaused;
};
