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

            sf::Vector2f vecBA = pA.pos - pB.pos;           // Вектор из B в A.
            float distanceMin = pA.radius + pB.radius;      // Расстояние между объектами, при котором нет удара.
            float vecBASqrLen = SqrLen(vecBA);
            if (vecBASqrLen < (distanceMin * distanceMin)) {
                std::cout << "Collision detected." << std::endl;

                float vecBALen = sqrt(vecBASqrLen);
                sf::Vector2f hitNormal = vecBA / vecBALen; // Вектор направления удара (всегда перпендикулярен касательной в точке удара, т.к. мы не моделируем трение между объектами).
                ResolveVelocity(pA, pB, hitNormal);

                float penetration = distanceMin - vecBALen;
                ResolvePenetration(pA, pB, penetration, hitNormal);
            }
        }

        HandleCollisionWithBorders(pA);
    }
}
/*
Здесь моделируется реакция объектов на столкновение.
Чтобы упростить рассмотрение, мы представляем,
что объект A сталкивается с неподвижной стеной, которая лежит в касательной плоскости, проходящей через точку удара между A и B.
Для этого мы переходим от двух абсолютных скоростей к одной относительной скорости A.
Поскольку мы моделируем столкновение абсолютно гладких и симметричных объектов,
то ударная сила, действующая на A, будет направлена перпендикулярно "стене".
В основе модели лежит предположение о соотношении относительной нормальной скорости до и после удара: Vrel' = -Vrel * c (гипотеза Ньютона).
Относительная скорость A после удара известна, но нам нужна дельта скорости для каждого объекта в отдельности.
В реальности время удара очень мало, поэтому внешние (по отношению к системе двух объектов) силы не меняют количества движения значительно.
Мы используем этот факт и игнорируем внешние силы в нашей модели.
Но тогда становится справедлив закон сохранения количества движения системы,
согласно которому изменение количества движения A будет равно по модулю и противоположно по направлению изменению B.
А изменения количества движения вызываются импульсами ударных сил, которые также равны и противоположны.
Для вычисления дельт скоростей нам нужно знать ударный импульс хотя бы для одного объекта.
Принятая нами гипотеза Ньютона позволяет вычислить такой импульс и применить его к каждому объекту.
Поскольку мы моделируем столкновение абстолютно гладких и симметричных объектов,
то все утверждения о векторных величинах относятся к нормальным проекциям, т.к. касательные составляющие не меняются.
*/
void PhysicalWorld::ResolveVelocity(Particle& pA, Particle& pB, sf::Vector2f hitNormal) {
    sf::Vector2f relVelA = pA.velocity - pB.velocity;
    float relVelANormalProj = Dot(relVelA, hitNormal);

    // Если объект уже отдаляется от точки удара, то повторно переключать скорости не надо.
    // Дело в том, что одну и ту же пару объектов мы можем обработать дважды за кадр.
    // При этом, объекты после первой обработки могут снова оказаться в состоянии удара.
    // Если мы еще раз переключим скорости, то такие объекты вместо отскока будут двигаться на повторное столкновение.
    if (relVelANormalProj >= 0)
        return;

    sf::Vector2f relVelANormal = relVelANormalProj * hitNormal;
    sf::Vector2f impulseANormal = -relVelANormal * ((RESTITUTION + 1) / (pA.invMass + pB.invMass));

    pA.velocity += impulseANormal * pA.invMass;
    pB.velocity += -impulseANormal * pB.invMass;
}

void PhysicalWorld::ResolvePenetration(Particle& pA, Particle& pB, float penetration, sf::Vector2f hitNormal) {
    // Если один из объектов неподвижен, то другой объект должен сместиться на полную величину проникновения.
    penetration = (pB.invMass <= 0.0f) ? penetration : penetration / 2.0f;

    pA.pos += hitNormal * penetration;
    pB.pos += -hitNormal * penetration;
}

void PhysicalWorld::HandleCollisionWithBorders(Particle& pA) {
    float penetration;
    sf::Vector2f hitNormal;
    bool coll = false;

    // Левая.
    if (pA.pos.x <= pA.radius) {
        std::cout << "Hit left" << std::endl;

        hitNormal = sf::Vector2f(1, 0);
        penetration = pA.radius - pA.pos.x;
        coll = true;
    }

    // Правая.
    if (pA.pos.x + pA.radius >= width) {
        std::cout << "Hit right" << std::endl;

        hitNormal = sf::Vector2f(-1, 0);
        penetration = (pA.pos.x + pA.radius) - width;
        coll = true;
    }

    // Верхняя.
    if (pA.pos.y <= pA.radius) {
        std::cout << "Hit up" << std::endl;

        hitNormal = sf::Vector2f(0, 1);
        penetration = pA.radius - pA.pos.y;
        coll = true;
    }

    // Нижняя.
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
