#include <iostream>

#include "Particle.h"

#include "App.h"

const sf::Time App::TimePerFrame = sf::seconds(1.f / 60.f);

App::App()
    : window(sf::VideoMode(640, 480), "Moving Circles"),
    shape(10) {
    std::cout << "App::App" << std::endl;

    shape.setFillColor(sf::Color::White);
}

void App::Run() {
    std::cout << "App::Run" << std::endl;

    Particle p1(1, sf::Vector2f(300, 240), 1);
    world.AddParticle(p1);
    Particle p2(1, sf::Vector2f(340, 240), 1);
    world.AddParticle(p2);

    sf::Clock clock;
    sf::Time sinceLastUpdate = sf::Time::Zero;

    while (window.isOpen()) {
        HandleEvents();

        sinceLastUpdate += clock.restart();
        while (sinceLastUpdate > TimePerFrame) {
            sinceLastUpdate -= TimePerFrame;
            HandleEvents();
            FixedUpdate(TimePerFrame);
        }

        Display();
    }
}

void App::HandleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void App::Display() {
    window.clear();

    for (auto const& p : world.particles) {
        shape.setPosition(p.pos);
        window.draw(shape);
    }

    window.display();
}

void App::FixedUpdate(sf::Time dt) {
    world.Step(dt.asSeconds());
}
