#include "Particle.h"

Particle::Particle(float mass, sf::Vector2f pos, float radius) {
    this->mass = mass;
    this->pos = pos;
    this->radius = radius;

    invMass = 1 / mass;
}

void Particle::SetPosition(sf::Vector2f pos) {
    this->pos = pos;
}

void Particle::ApplyForce(sf::Vector2f force) {
    this->force += force;
}
