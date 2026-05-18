// src/render/WaveRenderer.cpp
#include <cmath>
#include "render/WaveRenderer.h"

#ifndef M_PI
#define M_PI 3.1415926
#endif

// конструктор
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
      yaw(0.0f),
      pitch(0.3f),
      offset(600.0f, 400.0f)
{
    updateVertices();
}

// проекция 3d в 2d с перспективой
sf::Vector2f WaveRenderer::project(float x, float y, float z) const {
    // поворот вокруг y (yaw)
    float cos_yaw = std::cos(yaw);
    float sin_yaw = std::sin(yaw);
    float x_yaw = x * cos_yaw - z * sin_yaw;
    float z_yaw = x * sin_yaw + z * cos_yaw;

    // поворот вокруг x (pitch)
    float cos_pitch = std::cos(pitch);
    float sin_pitch = std::sin(pitch);
    float y_final = y * cos_pitch - z_yaw * sin_pitch;
    float z_final = y * sin_pitch + z_yaw * cos_pitch;

    // перспектива
    float perspective = 1000.0f / (1000.0f - z_final);

    return sf::Vector2f(
        offset.x + x_yaw * scale * perspective,
        offset.y - y_final * scale * perspective
    );
}

// обновление вертексов
void WaveRenderer::updateVertices() {
    float z_step = z_range / static_cast<float>(samples - 1);
    float current_time = wave.getTime();

    for (int i = 0; i < samples; i++) {
        float z = i * z_step - z_range / 2.0f;

        float e_val = wave.calculateE(current_time, z);
        float b_val = wave.calculateB(current_time, z);

        // e-поле по оси y
        sf::Vector2f pos_E = project(0.0f, e_val, z);
        vertices_E[i].position = pos_E;
        vertices_E[i].color = color_E;

        // b-поле по оси x
        sf::Vector2f pos_B = project(b_val, 0.0f, z);
        vertices_B[i].position = pos_B;
        vertices_B[i].color = color_B;
    }
}

// публичный update()
void WaveRenderer::update() {
    updateVertices();
}

// отрисовка с осями и линиями
void WaveRenderer::draw(sf::RenderWindow& window) const {
    // линии e-поля (красные)
    float z_step = z_range / static_cast<float>(samples - 1);
    float current_time = wave.getTime();

    int line_step = samples / 30;
    if (line_step < 1) line_step = 1;

    for (int i = 0; i < samples; i += line_step) {
        float z = i * z_step - z_range / 2.0f;
        float e_val = wave.calculateE(current_time, z);

        // точка на оси z
        sf::Vector2f axis_pos = project(0.0f, 0.0f, z);
        // точка на волне e
        sf::Vector2f wave_pos = project(0.0f, e_val, z);

        // линия от оси до волны
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = axis_pos;
        line[0].color = sf::Color(100, 50, 50);  // тёмно-красный
        line[1].position = wave_pos;
        line[1].color = sf::Color(255, 100, 100);  // светло-красный
        window.draw(line);
    }

    // линии b-поля (синие)
    for (int i = 0; i < samples; i += line_step) {
        float z = i * z_step - z_range / 2.0f;
        float b_val = wave.calculateB(current_time, z);

        // точка на оси z
        sf::Vector2f axis_pos = project(0.0f, 0.0f, z);
        // точка на волне b
        sf::Vector2f wave_pos = project(b_val, 0.0f, z);

        // линия от оси до волны
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = axis_pos;
        line[0].color = sf::Color(50, 50, 100);  // тёмно-синий
        line[1].position = wave_pos;
        line[1].color = sf::Color(100, 100, 255);  // светло-синий
        window.draw(line);
    }

    // оси координат (прямые)
    float axis_len = z_range * 0.6f;

    // ось x (красная)
    sf::VertexArray axisX(sf::Lines, 2);
    axisX[0].position = project(-axis_len, 0, 0);  // отрицательная сторона
    axisX[0].color = sf::Color(200, 100, 100);     // чуть темнее
    axisX[1].position = project(axis_len, 0, 0);   // положительная сторона
    axisX[1].color = sf::Color::Red;
    window.draw(axisX);

    // ось y (зелёная)
    sf::VertexArray axisY(sf::Lines, 2);
    axisY[0].position = project(0, -axis_len, 0);  // отрицательная сторона
    axisY[0].color = sf::Color(100, 200, 100);     // чуть темнее
    axisY[1].position = project(0, axis_len, 0);   // положительная сторона
    axisY[1].color = sf::Color::Green;
    window.draw(axisY);

    // ось z (синяя)
    sf::VertexArray axisZ(sf::Lines, 2);
    axisZ[0].position = project(0, 0, -axis_len);  // отрицательная сторона
    axisZ[0].color = sf::Color(100, 100, 200);     // чуть темнее
    axisZ[1].position = project(0, 0, axis_len);   // положительная сторона
    axisZ[1].color = sf::Color::Blue;
    window.draw(axisZ);

    // рисуем синусоиды
    window.draw(vertices_E);
    window.draw(vertices_B);
}
