#include <iostream>

#include "PhysicalWorld.h"
#include "App.h"

float Dot(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.x + a.y * b.y;
}

PhysicalWorld::PhysicalWorld() {
    std::cout << "PhysicsWorld::PhysicsWorld" << std::endl;
}

void PhysicalWorld::Step(float dt) {
    std::cout << "PhysicsWorld::Step" << std::endl;

    if (isPaused)
        return;

    for (auto& p : particles) {
        //sf::Vector2f dragForce = -p.velocity * 0.5f;
        //p.ApplyForce(dragForce);

        sf::Vector2f dv = ((p.force * p.invMass) /*+ sf::Vector2f(0, 768 * 2)*/) * dt;

        sf::Vector2f v0 = p.velocity;
        p.velocity += dv;

        p.pos += (v0 * dt) + (dv * dt) / 2.f;

        p.force = sf::Vector2f(0, 0);
    }

    for (auto& p1 : particles) {
        for (auto& p2 : particles) {
            if (&p1 == &p2)
                continue;

            sf::Vector2f distance = p1.pos - p2.pos;
            float sqrDistance = distance.x * distance.x + distance.y * distance.y;
            if (sqrDistance < App::RADIUS * 2 * App::RADIUS * 2) {
                std::cout << "Collision detected" << std::endl;

                // Resolve velocity
                sf::Vector2f normal = distance / sqrt(sqrDistance); // Нормаль удара из 2 в 1.
                sf::Vector2f relVel1 = p1.velocity - p2.velocity; // Скорость 1 относительно 2 (2 как неподвижный).

                float normalVel = Dot(relVel1, normal); // Проекция относительной скорости на нормаль удара.
                std::cout << "normalVel: " << std::to_string(normalVel) << std::endl;

                float impulseNormalProj = -normalVel * (1 + 1) / (p1.invMass + p2.invMass);

                sf::Vector2f impulse = impulseNormalProj * normal;
                p1.velocity += impulse * p1.invMass;
                p2.velocity += -impulse * p2.invMass;

                std::cout << "p1.velocity.x: " << std::to_string(p1.velocity.x) << std::endl;
                std::cout << "p2.velocity.x: " << std::to_string(p2.velocity.x) << std::endl;

                // Resolve penetration
                float penetration = App::RADIUS * 2 - sqrt(sqrDistance);
                std::cout << "penetration: " << std::to_string(penetration);

                p1.pos += normal * penetration;

                distance = p1.pos - p2.pos;
                std::cout << "new pen " << std::to_string(sqrt(distance.x * distance.x + distance.y * distance.y));

                //isPaused = true;
            }
        }

        sf::Vector2f normal;
        float penetration;
        bool coll = false;

        // Проверяем удар с левой стенкой
        if (p1.pos.x <= App::RADIUS) {
            std::cout << "left coll" << std::endl;

            normal = sf::Vector2f(1, 0);
            penetration = App::RADIUS - p1.pos.x;
            coll = true;
        }

        // Проверяем удар с правой стенкой
        if (p1.pos.x + App::RADIUS >= 1024) {
            std::cout << "right coll" << std::endl;

            normal = sf::Vector2f(-1, 0);
            penetration = (p1.pos.x + App::RADIUS) - 1024;
            coll = true;
        }

        // Проверяем удар с верхней стенкой
        if (p1.pos.y <= App::RADIUS) {
            std::cout << "up coll" << std::endl;

            normal = sf::Vector2f(0, 1);
            penetration = App::RADIUS - p1.pos.y;
            coll = true;
        }

        // Проверяем удар с нижней стенкой
        if (p1.pos.y + App::RADIUS >= 768) {
            std::cout << "down coll" << std::endl;

            normal = sf::Vector2f(0, -1);
            penetration = (p1.pos.y + App::RADIUS) - 768;
            coll = true;
        }

        if (coll) {
            // Resolve velocity
            sf::Vector2f relVel1 = p1.velocity;

            float normalVel = Dot(relVel1, normal); // Проекция относительной скорости на нормаль удара.
            std::cout << "normalVel: " << std::to_string(normalVel) << std::endl;

            float impulseNormalProj = -normalVel * (1 + 1) / (p1.invMass + 0);

            sf::Vector2f impulse = impulseNormalProj * normal;
            p1.velocity += impulse * p1.invMass;

            std::cout << "p1.velocity.x: " << std::to_string(p1.velocity.x) << std::endl;

            // Resolve penetration
            std::cout << "penetration: " << std::to_string(penetration);

            p1.pos += normal * penetration;
        }
    }
}

void PhysicalWorld::AddParticle(Particle& particle) {
    particles.push_back(particle);
}
