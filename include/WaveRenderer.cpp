// src/render/WaveRenderer.cpp
#include <cmath>
#include "render/WaveRenderer.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


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
      offset(400.0f, 300.0f)
{
    updateVertices();
}


sf::Vector2f WaveRenderer::project(float x, float y, float z) const {
    float rad = cam_angle * M_PI / 180.0f;

    // Поворот вокруг оси Y (X и Z вращаются, Y остаётся)
    float x_rot = x * std::cos(rad) - z * std::sin(rad);
    // z_rot не нужен для 2D-проекции

    // Проекция на экран
    // Y инвертирован, т.к. в SFML ось Y направлена вниз
    return sf::Vector2f(
        offset.x + x_rot * scale,
        offset.y - y * scale  // ← ✅ Исправлено: y, не y_rot
    );
}


void WaveRenderer::updateVertices() {
    float z_step = z_range / static_cast<float>(samples - 1);
    float current_time = wave.getTime();

    for (int i = 0; i < samples; i++) {
        float z = i * z_step - z_range / 2.0f;

        float e_val = wave.calculateE(current_time, z);  // E-поле (ось Y)
        float b_val = wave.calculateB(current_time, z);  // B-поле (ось X)

        // E-поле: колеблется по Y
        sf::Vector2f pos_E = project(0.0f, e_val, z);
        vertices_E[i].position = pos_E;
        vertices_E[i].color = color_E;

        // B-поле: колеблется по X
        sf::Vector2f pos_B = project(b_val, 0.0f, z);
        vertices_B[i].position = pos_B;
        vertices_B[i].color = color_B;
    }
}


void WaveRenderer::update() {
    updateVertices();
}


void WaveRenderer::draw(sf::RenderWindow& window) const {
    window.draw(vertices_E);
    window.draw(vertices_B);
}
