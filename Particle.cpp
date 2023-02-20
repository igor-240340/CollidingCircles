#include "Particle.h"

Particle::Particle(float mass, sf::Vector2f pos, float radius) {
    invMass = 1 / mass;

    this->pos = pos;
    this->radius = radius;
}

Particle::Particle(sf::Vector2f pos, sf::Vector2f vel, float mass, float radius) {
    this->pos = pos;
    this->velocity = vel;
    this->radius = radius;

    invMass = 1 / mass;
}

void Particle::SetPosition(sf::Vector2f pos) {
    this->pos = pos;
}

void Particle::ApplyForce(sf::Vector2f force) {
    this->force += force;
}
