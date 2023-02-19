#include <iostream>

#include "PhysicalWorld.h"

PhysicalWorld::PhysicalWorld() {
    std::cout << "PhysicsWorld::PhysicsWorld" << std::endl;
}

void PhysicalWorld::Step(float dt) {
    std::cout << "PhysicsWorld::Step" << std::endl;
}

void PhysicalWorld::AddParticle(Particle& particle) {
    particles.push_back(particle);
}
