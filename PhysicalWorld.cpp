#include <iostream>

#include "PhysicalWorld.h"
#include "App.h"

float Dot(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.x + a.y * b.y;
}

PhysicalWorld::PhysicalWorld() {
    std::cout << "PhysicsWorld::PhysicsWorld" << std::endl;
}

void PhysicalWorld::AddParticle(Particle& particle) {
    particles.push_back(particle);
}

void PhysicalWorld::Step(float dt) {
    if (isPaused)
        return;

    for (auto& p : particles) {
        sf::Vector2f dragForce = -p.velocity * 0.5f;
        //p.ApplyForce(dragForce);

        sf::Vector2f dv = (p.force * p.invMass) * dt;

        sf::Vector2f v0 = p.velocity;
        p.velocity += dv;

        // Обновляем позицию с учетом того, что скорость в течение dt была переменной.
        p.pos += (v0 * dt) + (dv * dt) / 2.0f;

        p.force = sf::Vector2f(0, 0);
    }

    for (auto& pA : particles) {
        for (auto& pB : particles) {
            if (&pA == &pB)
                continue;

            sf::Vector2f distance = pA.pos - pB.pos;
            float sqrDistance = distance.x * distance.x + distance.y * distance.y;
            if (sqrDistance < (pA.radius + pB.radius) * (pA.radius + pB.radius)) {
                std::cout << "Collision detected" << std::endl;

                // Resolve velocity
                sf::Vector2f normal = distance / sqrt(sqrDistance); // Нормаль удара из 2 в 1.
                sf::Vector2f relVel1 = pA.velocity - pB.velocity; // Скорость 1 относительно 2 (2 как неподвижный).

                float normalVel = Dot(relVel1, normal); // Проекция относительной скорости на нормаль удара.
                std::cout << "normalVel: " << std::to_string(normalVel) << std::endl;

                float impulseNormalProj = -normalVel * (1 + 1) / (pA.invMass + pB.invMass);

                sf::Vector2f impulse = impulseNormalProj * normal;
                pA.velocity += impulse * pA.invMass;
                pB.velocity += -impulse * pB.invMass;

                std::cout << "p1.velocity.x: " << std::to_string(pA.velocity.x) << std::endl;
                std::cout << "p2.velocity.x: " << std::to_string(pB.velocity.x) << std::endl;

                // Resolve penetration
                float penetration = pA.radius + pB.radius - sqrt(sqrDistance);
                std::cout << "penetration: " << std::to_string(penetration);

                pA.pos += normal * penetration;

                distance = pA.pos - pB.pos;
                std::cout << "new pen " << std::to_string(sqrt(distance.x * distance.x + distance.y * distance.y));

                //isPaused = true;
            }
        }

        sf::Vector2f normal;
        float penetration;
        bool coll = false;

        // Проверяем удар с левой стенкой
        if (pA.pos.x <= pA.radius) {
            std::cout << "left coll" << std::endl;

            normal = sf::Vector2f(1, 0);
            penetration = pA.radius - pA.pos.x;
            coll = true;
        }

        // Проверяем удар с правой стенкой
        if (pA.pos.x + pA.radius >= 1024) {
            std::cout << "right coll" << std::endl;

            normal = sf::Vector2f(-1, 0);
            penetration = (pA.pos.x + pA.radius) - 1024;
            coll = true;
        }

        // Проверяем удар с верхней стенкой
        if (pA.pos.y <= pA.radius) {
            std::cout << "up coll" << std::endl;

            normal = sf::Vector2f(0, 1);
            penetration = pA.radius - pA.pos.y;
            coll = true;
        }

        // Проверяем удар с нижней стенкой
        if (pA.pos.y + pA.radius >= 768) {
            std::cout << "down coll" << std::endl;

            normal = sf::Vector2f(0, -1);
            penetration = (pA.pos.y + pA.radius) - 768;
            coll = true;
        }

        if (coll) {
            // Resolve velocity
            sf::Vector2f relVel1 = pA.velocity;

            float normalVel = Dot(relVel1, normal); // Проекция относительной скорости на нормаль удара.
            std::cout << "normalVel: " << std::to_string(normalVel) << std::endl;

            float impulseNormalProj = -normalVel * (1 + 1) / (pA.invMass + 0);

            sf::Vector2f impulse = impulseNormalProj * normal;
            pA.velocity += impulse * pA.invMass;

            std::cout << "p1.velocity.x: " << std::to_string(pA.velocity.x) << std::endl;

            // Resolve penetration
            std::cout << "penetration: " << std::to_string(penetration);

            pA.pos += normal * penetration;
        }
    }
}
