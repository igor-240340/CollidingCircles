#include <iostream>

#include "Particle.h"

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, float mass, float radius) {
    this->pos = pos;
    this->velocity = vel;
    this->radius = radius;

    this->mass = mass;
    invMass = 1 / mass;
}

Particle::Particle(float invMass) {
    this->invMass = invMass;
}

void Particle::SetPosition(sf::Vector2f pos) {
    this->pos = pos;
}

void Particle::ApplyForce(sf::Vector2f force) {
    this->force += force;
}
