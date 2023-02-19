#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Particle.h"

#include "App.h"

const sf::Time App::SECS_PER_FRAME = sf::seconds(1.f / 60.f);
const int App::RADIUS = 50;

App::App()
    : window(sf::VideoMode(1024, 768), "Moving Circles")
    , shape(RADIUS)
    , mFont()
    , mStatisticsText()
    , mStatisticsUpdateTime()
    , mStatisticsNumFrames(0)
{

    mFont.loadFromFile("Sansation.ttf");
    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(20);
    mStatisticsText.setFillColor(sf::Color::White);

    //std::cout << "App::App" << std::endl;

    shape.setFillColor(sf::Color::White);
}

void App::Run() {
    std::cout << "App::Run" << std::endl;

    std::srand(std::time(nullptr));

    sf::Clock clock;
    sf::Time sinceLastUpdate = sf::Time::Zero;

    Particle p1(1, sf::Vector2f(500, 234), 10);
    p1.velocity = sf::Vector2f(400, 0);
    p1.isMarker = true;
    world.AddParticle(p1);

    while (window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        sinceLastUpdate += elapsedTime;
        while (sinceLastUpdate > SECS_PER_FRAME) {
            HandleEvents();
            sinceLastUpdate -= SECS_PER_FRAME;
            FixedUpdate(SECS_PER_FRAME);
        }

        UpdateStat(elapsedTime);
        Display();
    }
}

void App::UpdateStat(sf::Time elapsedTime)
{
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames += 1;

    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString(
            "Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
            "Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void App::HandleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyReleased)
        {
            //Particle p2(1, sf::Vector2f(512, 384), 10);

            //p2.velocity = sf::Vector2f(100, 300);

            //world.AddParticle(p2);

            for (int i = 0; i < 5; i++) {
                int angleDeg = std::rand() % 180;
                float angleRad = angleDeg * (3.14 / 180);
                float magnitude = std::rand() % 1000 + 1;
                sf::Vector2f randVel = { cos(angleRad), sin(angleRad) };
                randVel = randVel * magnitude;
                sf::Vector2f randPos;
                randPos.x = std::rand() % 1024;
                randPos.y = std::rand() % 768;

                Particle p1(1, randPos, 1);
                p1.velocity = randVel;
                world.AddParticle(p1);
            }
        }
    }
}

void App::Display() {
    window.clear();

    for (auto const& p : world.particles) {
        if (p.isMarker) {
            shape.setFillColor(sf::Color::Cyan);
        } else
            shape.setFillColor(sf::Color::White);
        shape.setPosition(sf::Vector2f(p.pos.x - RADIUS, p.pos.y - RADIUS));
        window.draw(shape);
    }
    window.draw(mStatisticsText);
    window.display();
}

void App::FixedUpdate(sf::Time dt) {
    world.Step(dt.asSeconds());
}
