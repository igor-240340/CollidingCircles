#include <iostream>
#include <cstdlib>
#include <ctime>

#include "App.h"
#include "Particle.h"

const sf::Time App::FIXED_DELTA_TIME = sf::seconds(1.0f / 60.0f);

App::App() {
    window = new sf::RenderWindow(sf::VideoMode(1024, 768), "Moving Circles");
    circleShape = new sf::CircleShape();
    circleShape->setFillColor(sf::Color::Red);

    std::srand(std::time(nullptr));
}

void App::Run() {
    FillWorldWithCircles();

    sf::Clock clock;
    sf::Time dtAcc = sf::Time::Zero;

    while (window->isOpen()) {
        sf::Time dt = clock.restart();
        dtAcc += dt;
        while (dtAcc > FIXED_DELTA_TIME) {
            HandleInput();
            FixedUpdate(FIXED_DELTA_TIME);
            dtAcc -= FIXED_DELTA_TIME;
        }

        Render();
    }
}

void App::HandleInput() {
    sf::Event event;

    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window->close();

        //if (event.type == sf::Event::KeyReleased)
    }
}

void App::FixedUpdate(sf::Time dt) {
    world.Step(dt.asSeconds());
}

void App::Render() {
    window->clear();

    for (auto const& p : world.particles) {
        circleShape->setRadius(p.radius);
        circleShape->setPosition(sf::Vector2f(p.pos.x - p.radius, p.pos.y - p.radius));
        window->draw(*circleShape);
    }

    window->display();
}

void App::FillWorldWithCircles() {
    for (int i = 0; i < App::CIRCLE_COUNT; i++) {
        sf::Vector2f randPos(std::rand() % 1024, std::rand() % 768);
        sf::Vector2f randVel = randPos / 2.0f;
        float randMass = std::rand() % 50 + 5;
        float randRadius = randMass;

        Particle p(randPos, randVel, randMass, randRadius);
        world.AddParticle(p);
    }
}

App::~App() {
    delete window;
    delete circleShape;
}
