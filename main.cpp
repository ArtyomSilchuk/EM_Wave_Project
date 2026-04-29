// main.cpp
#include <SFML/Graphics.hpp>
#include <iostream>
#include "physics/EMWave.h"
#include "render/WaveRenderer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 800), "EM Wave Simulator - 3D");
    window.setFramerateLimit(60);

    // === ИСПРАВЛЕНО: более низкая частота ===
    EMWave wave(
        0.5f,   // ← Частота: было 2.0, стало 0.5 (меньше периодов)
        6.0f,   // ← Длина волны: было 3.0, стало 6.0 (длиннее)
        100.0f, // ← Амплитуда E: увеличена для наглядности
        100.0f, // ← Амплитуда B: увеличена для наглядности
        0.0f
    );

    // === ИСПРАВЛЕНО: меньше точек, больше диапазон ===
    WaveRenderer renderer(
        wave,
        1.0f,   // scale
        200,    // ← samples: было 400, стало 200 (меньше точек)
        1200.0f // ← z_range: было 800, стало 1200 (длиннее волна)
    );

    renderer.setColors(sf::Color::Red, sf::Color::Blue);
    renderer.setOffset(sf::Vector2f(600.0f, 400.0f));

    sf::Clock clock;
    float time_scale = 0.5f;  // ← Замедлено для наглядности
    bool paused = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();

                if (event.key.code == sf::Keyboard::Left)
                    renderer.setYaw(renderer.getYaw() - 0.1f);
                if (event.key.code == sf::Keyboard::Right)
                    renderer.setYaw(renderer.getYaw() + 0.1f);
                if (event.key.code == sf::Keyboard::Up)
                    renderer.setPitch(renderer.getPitch() + 0.05f);
                if (event.key.code == sf::Keyboard::Down)
                    renderer.setPitch(renderer.getPitch() - 0.05f);

                if (event.key.code == sf::Keyboard::Q)
                    time_scale *= 0.8f;
                if (event.key.code == sf::Keyboard::E)
                    time_scale *= 1.25f;

                if (event.key.code == sf::Keyboard::Space)
                    paused = !paused;
            }
        }

        if (!paused) {
            wave.update(dt * time_scale);
            renderer.update();
        }

        window.clear(sf::Color::Black);
        renderer.draw(window);
        window.display();
    }

    return 0;
}
