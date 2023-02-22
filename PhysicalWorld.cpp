#include <iostream>

#include "PhysicalWorld.h"

const float PhysicalWorld::DRAG_COEFF = 0.5f;
const float PhysicalWorld::RESTITUTION = 1.0f;

PhysicalWorld::PhysicalWorld(float width, float height) {
    this->width = width;
    this->height = height;
}

void PhysicalWorld::AddParticle(Particle& particle) {
    particles.push_back(particle);
}

const std::list<Particle>& PhysicalWorld::Particles() {
    return particles;
}

void PhysicalWorld::Step(float dt) {
    for (auto& p : particles) {
        sf::Vector2f dragForce = -p.velocity * DRAG_COEFF;
        p.ApplyForce(dragForce);

        sf::Vector2f dv = (p.force * p.invMass) * dt;

        sf::Vector2f v0 = p.velocity;
        p.velocity += dv;

        p.pos += (v0 * dt) + (dv * dt) / 2.0f;

        p.force = sf::Vector2f(0, 0);
    }

    HandleCollision();
}

void PhysicalWorld::HandleCollision() {
    for (auto& pA : particles) {
        for (auto& pB : particles) {
            if (&pA == &pB)
                continue;

            sf::Vector2f vecBA = pA.pos - pB.pos;           // ������ �� B � A.
            float distanceMin = pA.radius + pB.radius;      // ���������� ����� ���������, ��� ������� ��� �����.
            float vecBASqrLen = SqrLen(vecBA);
            if (vecBASqrLen < (distanceMin * distanceMin)) {
                std::cout << "Collision detected." << std::endl;

                float vecBALen = sqrt(vecBASqrLen);
                sf::Vector2f hitNormal = vecBA / vecBALen; // ������ ����������� ����� (������ ��������������� ����������� � ����� �����, �.�. �� �� ���������� ������ ����� ���������).
                ResolveVelocity(pA, pB, hitNormal);

                float penetration = distanceMin - vecBALen;
                ResolvePenetration(pA, pB, penetration, hitNormal);
            }
        }

        HandleCollisionWithBorders(pA);
    }
}
/*
����� ������������ ������� �������� �� ������������.
����� ��������� ������������, �� ������������,
��� ������ A ������������ � ����������� ������, ������� ����� � ����������� ���������, ���������� ����� ����� ����� ����� A � B.
��� ����� �� ��������� �� ���� ���������� ��������� � ����� ������������� �������� A.
��������� �� ���������� ������������ ��������� ������� � ������������ ��������,
�� ������� ����, ����������� �� A, ����� ���������� ��������������� "�����".
� ������ ������ ����� ������������� � ����������� ������������� ���������� �������� �� � ����� �����: Vrel' = -Vrel * c (�������� �������).
������������� �������� A ����� ����� ��������, �� ��� ����� ������ �������� ��� ������� ������� � �����������.
� ���������� ����� ����� ����� ����, ������� ������� (�� ��������� � ������� ���� ��������) ���� �� ������ ���������� �������� �����������.
�� ���������� ���� ���� � ���������� ������� ���� � ����� ������.
�� ����� ���������� ���������� ����� ���������� ���������� �������� �������,
�������� �������� ��������� ���������� �������� A ����� ����� �� ������ � �������������� �� ����������� ��������� B.
� ��������� ���������� �������� ���������� ���������� ������� ���, ������� ����� ����� � ��������������.
��� ���������� ����� ��������� ��� ����� ����� ������� ������� ���� �� ��� ������ �������.
�������� ���� �������� ������� ��������� ��������� ����� ������� � ��������� ��� � ������� �������.
��������� �� ���������� ������������ ���������� ������� � ������������ ��������,
�� ��� ����������� � ��������� ��������� ��������� � ���������� ���������, �.�. ����������� ������������ �� ��������.
*/
void PhysicalWorld::ResolveVelocity(Particle& pA, Particle& pB, sf::Vector2f hitNormal) {
    sf::Vector2f relVelA = pA.velocity - pB.velocity;
    float relVelANormalProj = Dot(relVelA, hitNormal);

    // ���� ������ ��� ���������� �� ����� �����, �� �������� ����������� �������� �� ����.
    // ���� � ���, ��� ���� � �� �� ���� �������� �� ����� ���������� ������ �� ����.
    // ��� ����, ������� ����� ������ ��������� ����� ����� ��������� � ��������� �����.
    // ���� �� ��� ��� ���������� ��������, �� ����� ������� ������ ������� ����� ��������� �� ��������� ������������.
    if (relVelANormalProj >= 0)
        return;

    sf::Vector2f relVelANormal = relVelANormalProj * hitNormal;
    sf::Vector2f impulseANormal = -relVelANormal * ((RESTITUTION + 1) / (pA.invMass + pB.invMass));

    pA.velocity += impulseANormal * pA.invMass;
    pB.velocity += -impulseANormal * pB.invMass;
}

void PhysicalWorld::ResolvePenetration(Particle& pA, Particle& pB, float penetration, sf::Vector2f hitNormal) {
    // ���� ���� �� �������� ����������, �� ������ ������ ������ ���������� �� ������ �������� �������������.
    penetration = (pB.invMass <= 0.0f) ? penetration : penetration / 2.0f;

    pA.pos += hitNormal * penetration;
    pB.pos += -hitNormal * penetration;
}

void PhysicalWorld::HandleCollisionWithBorders(Particle& pA) {
    float penetration;
    sf::Vector2f hitNormal;
    bool coll = false;

    // �����.
    if (pA.pos.x <= pA.radius) {
        std::cout << "Hit left" << std::endl;

        hitNormal = sf::Vector2f(1, 0);
        penetration = pA.radius - pA.pos.x;
        coll = true;
    }

    // ������.
    if (pA.pos.x + pA.radius >= width) {
        std::cout << "Hit right" << std::endl;

        hitNormal = sf::Vector2f(-1, 0);
        penetration = (pA.pos.x + pA.radius) - width;
        coll = true;
    }

    // �������.
    if (pA.pos.y <= pA.radius) {
        std::cout << "Hit up" << std::endl;

        hitNormal = sf::Vector2f(0, 1);
        penetration = pA.radius - pA.pos.y;
        coll = true;
    }

    // ������.
    if (pA.pos.y + pA.radius >= height) {
        std::cout << "Hit bottom" << std::endl;

        hitNormal = sf::Vector2f(0, -1);
        penetration = (pA.pos.y + pA.radius) - height;
        coll = true;
    }

    if (coll) {
        Particle infMassObject(0.0f);
        infMassObject.velocity = sf::Vector2f(0, 0);

        ResolveVelocity(pA, infMassObject, hitNormal);
        ResolvePenetration(pA, infMassObject, penetration, hitNormal);
    }
}

float PhysicalWorld::Dot(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.x + a.y * b.y;
}

float PhysicalWorld::SqrLen(sf::Vector2f v) {
    return v.x * v.x + v.y * v.y;
}
