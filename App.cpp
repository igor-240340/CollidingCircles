#include <iostream>
#include <cstdlib>
#include <ctime>

#include "App.h"
#include "Particle.h"

const sf::Time App::FIXED_DELTA_TIME = sf::seconds(1.0f / 60.0f);
const sf::Color App::BACKGROUND_COLOR = sf::Color(235, 235, 235);
const sf::Color App::CIRCLE_COLOR = sf::Color(0, 152, 156);

App::App() :world(WIDTH, HEIGHT) {
    window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Moving Circles");

    circleShape = new sf::CircleShape();
    circleShape->setFillColor(CIRCLE_COLOR);

    std::srand(std::time(nullptr));
}

App::~App() {
    delete window;
    delete circleShape;
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

void App::FillWorldWithCircles() {
    for (int i = 0; i < App::CIRCLE_COUNT; i++) {
        // Так объекты могут появиться на границах экрана, но физика их от туда все-равно "вытащит".
        sf::Vector2f randPos(std::rand() % WIDTH, std::rand() % HEIGHT);

        // Для простоты за вектор начальной скорости возьмем вектор позиции.
        sf::Vector2f randVel = randPos / 2.0f;

        // Более массивные элементы будут иметь больший радиус.
        float randMass = std::rand() % 100 + 5;
        float randRadius = randMass;

        Particle p(randPos, randVel, randMass, randRadius);
        world.AddParticle(p);
    }
}

void App::HandleInput() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window->close();
    }
}

void App::FixedUpdate(sf::Time dt) {
    world.Step(dt.asSeconds());
}

void App::Render() {
    window->clear(BACKGROUND_COLOR);

    for (auto const& p : world.Particles()) {
        circleShape->setRadius(p.radius);
        circleShape->setPosition(sf::Vector2f(p.pos.x - p.radius, p.pos.y - p.radius));
        window->draw(*circleShape);
    }

    window->display();
}
