// main.cpp
#include <SFML/Graphics.hpp>
#include "physics/EMWave.h"
#include "render/WaveRenderer.h"

int main() {
    // Создаём окно SFML
    sf::RenderWindow window(sf::VideoMode(1200, 800), "EM Wave Simulator");
    window.setFramerateLimit(60);

    // Создаём модель волны
    EMWave wave(
        0.5f,   // frequency (Гц)
        400.0f,   // wavelength (м)
        100.0f,  // amplitude_E
        100.0f,  // amplitude_B
        0.0f    // phase
    );

    // Создаём рендерер
    WaveRenderer renderer(
        wave,
        1.0f,    // scale: 1 физ.единица = 1 пиксель
        500,     // samples: количество точек для отрисовки
        800.0f   // z_range: диапазон по оси Z
    );
    renderer.setColors(sf::Color::Red, sf::Color::Cyan);
    renderer.setOffset(sf::Vector2f(600.0f, 400.0f));  // Центр экрана

    sf::Clock clock;

    // Главный цикл
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        // Обработка событий
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Управление клавишами
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                if (event.key.code == sf::Keyboard::Up) {
                    wave.setFrequency(wave.getFrequency() + 0.5f);
                }
                if (event.key.code == sf::Keyboard::Down) {
                    wave.setFrequency(wave.getFrequency() - 0.5f);
                }
            }
        }

        // Обновление
        wave.update(dt);
        renderer.update();

        // Отрисовка
        window.clear(sf::Color::Black);

        // Рисуем оси координат (для наглядности)
        sf::VertexArray axes(sf::Lines, 6);
        axes[0].position = sf::Vector2f(100, 400);
        axes[0].color = sf::Color::White;
        axes[1].position = sf::Vector2f(1100, 400);
        axes[1].color = sf::Color::White;
        axes[2].position = sf::Vector2f(600, 100);
        axes[2].color = sf::Color::White;
        axes[3].position = sf::Vector2f(600, 700);
        axes[3].color = sf::Color::White;
        axes[4].position = sf::Vector2f(600, 400);
        axes[4].color = sf::Color::Yellow;
        axes[5].position = sf::Vector2f(800, 300);
        axes[5].color = sf::Color::Yellow;
        window.draw(axes);

        // Рисуем волну
        renderer.draw(window);

        // Рисуем информацию
        sf::Text info;
        // (Здесь можно добавить вывод текста, если подключён шрифт)

        window.display();
    }

    return 0;
}
