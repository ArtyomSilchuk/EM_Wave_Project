// src/render/WaveRenderer.cpp
#include <cmath>
#include "render/WaveRenderer.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ============================================================
// Конструктор
// ============================================================
WaveRenderer::WaveRenderer(const EMWave& em_wave,
                           float scale,
                           int samples,
                           float z_range)
    : wave(em_wave),
      color_E(sf::Color::Red),
      color_B(sf::Color::Blue),
      scale(scale),
      samples(samples),
      z_range(z_range),
      vertices_E(sf::LinesStrip, samples),
      vertices_B(sf::LinesStrip, samples),
      cam_angle(30.0f),
      offset(1600.0f, 1500.0f)
{
    updateVertices();
}

// ============================================================
// Проекция 3D → 2D
// ============================================================
sf::Vector2f WaveRenderer::project(float x, float y, float z) const {
    float rad = cam_angle * M_PI / 180.0f;

    float x_rot = x * std::cos(rad) - z * std::sin(rad);

    return sf::Vector2f(
        offset.x + x_rot * scale,
        offset.y - y * scale
    );
}

// ============================================================
// Обновление вертексов
// ============================================================
void WaveRenderer::updateVertices() {
    float z_step = z_range / static_cast<float>(samples - 1);
    float current_time = wave.getTime();

    for (int i = 0; i < samples; i++) {
        float z = i * z_step - z_range / 2.0f;

        float e_val = wave.calculateE(current_time, z);
        float b_val = wave.calculateB(current_time, z);

        sf::Vector2f pos_E = project(0.0f, e_val, z);
        vertices_E[i].position = pos_E;
        vertices_E[i].color = color_E;

        sf::Vector2f pos_B = project(b_val, 0.0f, z);
        vertices_B[i].position = pos_B;
        vertices_B[i].color = color_B;
    }
}

// ============================================================
// Публичный update()
// ============================================================
void WaveRenderer::update() {
    updateVertices();
}

// ============================================================
// Отрисовка
// ============================================================
void WaveRenderer::draw(sf::RenderWindow& window) const {
    window.draw(vertices_E);
    window.draw(vertices_B);
}
