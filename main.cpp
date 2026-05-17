// main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include "physics/EMWave.h"
#include "render/WaveRenderer.h"
#include "ui/UIPanel.h" // ?????????? ????? ??????

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "EM Wave Simulator - 3D");
    window.setFramerateLimit(60);

    // ???????? ?????? (??????????? ??????? ???? ? ????????????? .ttf ?????)
    sf::Font font;
    if (!font.loadFromFile("resources/arial.ttf")) {
        std::cerr << "Failed to load font. Check path!" << std::endl;
        return -1;
    }

    EMWave wave(0.5f, 6.0f, 100.0f, 100.0f, 0.0f);
    WaveRenderer renderer(wave, 1.0f, 200, 1200.0f);
    renderer.setColors(sf::Color::Red, sf::Color::Blue);
    renderer.setOffset(sf::Vector2f(600.0f, 400.0f));

    UIPanel panel(window, font);
    //panel.updateInfo(wave); // ????????? ????????????? ??????

    sf::Clock clock;
    float time_scale = 1.0f;
    bool paused = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            panel.handleEvent(event, wave); // ????????? ?????????

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::Left)  renderer.setYaw(renderer.getYaw() - 0.1f);
                if (event.key.code == sf::Keyboard::Right) renderer.setYaw(renderer.getYaw() + 0.1f);
                if (event.key.code == sf::Keyboard::Up)    renderer.setPitch(renderer.getPitch() + 0.05f);
                if (event.key.code == sf::Keyboard::Down)  renderer.setPitch(renderer.getPitch() - 0.05f);
                if (event.key.code == sf::Keyboard::Q)     time_scale *= 0.8f;
                if (event.key.code == sf::Keyboard::E)     time_scale *= 1.25f;
                if (event.key.code == sf::Keyboard::Space) paused = !paused;
            }
        }

        if (!paused) {
            wave.update(dt * time_scale);
            renderer.update();
        }

        //panel.updateInfo(wave); // ????????? ????????? ???????? ?? ??????

        window.clear(sf::Color::Black);
        renderer.draw(window);

        window.setView(window.getDefaultView());
        panel.draw();           // ?????? ?????? ?????? ?????
        window.display();
    }

    return 0;
}