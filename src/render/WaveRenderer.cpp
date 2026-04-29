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
      yaw(0.0f),
      pitch(0.3f),
      offset(600.0f, 400.0f)
{
    updateVertices();
}

// ============================================================
// Проекция 3D → 2D с перспективой
// ============================================================
sf::Vector2f WaveRenderer::project(float x, float y, float z) const {
    // 1. Поворот вокруг оси Y (yaw)
    float cos_yaw = std::cos(yaw);
    float sin_yaw = std::sin(yaw);
    float x_yaw = x * cos_yaw - z * sin_yaw;
    float z_yaw = x * sin_yaw + z * cos_yaw;

    // 2. Поворот вокруг оси X (pitch)
    float cos_pitch = std::cos(pitch);
    float sin_pitch = std::sin(pitch);
    float y_final = y * cos_pitch - z_yaw * sin_pitch;
    float z_final = y * sin_pitch + z_yaw * cos_pitch;

    // 3. Перспектива
    float perspective = 1000.0f / (1000.0f - z_final);

    return sf::Vector2f(
        offset.x + x_yaw * scale * perspective,
        offset.y - y_final * scale * perspective
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

        // E-поле: колеблется по оси Y
        sf::Vector2f pos_E = project(0.0f, e_val, z);
        vertices_E[i].position = pos_E;
        vertices_E[i].color = color_E;

        // B-поле: колеблется по оси X
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
// Отрисовка с осями и соединительными линиями
// ============================================================
// ============================================================
// Отрисовка с осями и соединительными линиями
// ============================================================
void WaveRenderer::draw(sf::RenderWindow& window) const {
    // 1. Рисуем соединительные линии для E-поля (вертикальные, красные)
    float z_step = z_range / static_cast<float>(samples - 1);
    float current_time = wave.getTime();

    int line_step = samples / 30;  // Рисуем линии через каждые N точек
    if (line_step < 1) line_step = 1;

    for (int i = 0; i < samples; i += line_step) {
        float z = i * z_step - z_range / 2.0f;
        float e_val = wave.calculateE(current_time, z);

        // Точка на оси Z (центр)
        sf::Vector2f axis_pos = project(0.0f, 0.0f, z);
        // Точка на волне E
        sf::Vector2f wave_pos = project(0.0f, e_val, z);

        // Линия от оси до волны
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = axis_pos;
        line[0].color = sf::Color(100, 50, 50);  // Тёмно-красный для оси
        line[1].position = wave_pos;
        line[1].color = sf::Color(255, 100, 100);  // Светло-красный для линии
        window.draw(line);
    }

    // 2. Рисуем соединительные линии для B-поля (горизонтальные, синие)
    for (int i = 0; i < samples; i += line_step) {
        float z = i * z_step - z_range / 2.0f;
        float b_val = wave.calculateB(current_time, z);

        // Точка на оси Z (центр)
        sf::Vector2f axis_pos = project(0.0f, 0.0f, z);
        // Точка на волне B
        sf::Vector2f wave_pos = project(b_val, 0.0f, z);

        // Линия от оси до волны
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = axis_pos;
        line[0].color = sf::Color(50, 50, 100);  // Тёмно-синий для оси
        line[1].position = wave_pos;
        line[1].color = sf::Color(100, 100, 255);  // Светло-синий для линии
        window.draw(line);
    }

    // 3. Рисуем оси координат (ПОЛНЫЕ ПРЯМЫЕ, не лучи)
    float axis_len = z_range * 0.6f;

    // === Ось X (красная, влево-вправо) ===
    sf::VertexArray axisX(sf::Lines, 2);
    axisX[0].position = project(-axis_len, 0, 0);  // ← Отрицательная сторона
    axisX[0].color = sf::Color(200, 100, 100);     // Чуть темнее
    axisX[1].position = project(axis_len, 0, 0);   // ← Положительная сторона
    axisX[1].color = sf::Color::Red;
    window.draw(axisX);

    // === Ось Y (зелёная, вниз-вверх) ===
    sf::VertexArray axisY(sf::Lines, 2);
    axisY[0].position = project(0, -axis_len, 0);  // ← Отрицательная сторона
    axisY[0].color = sf::Color(100, 200, 100);     // Чуть темнее
    axisY[1].position = project(0, axis_len, 0);   // ← Положительная сторона
    axisY[1].color = sf::Color::Green;
    window.draw(axisY);

    // === Ось Z (синяя, назад-вперёд) ===
    sf::VertexArray axisZ(sf::Lines, 2);
    axisZ[0].position = project(0, 0, -axis_len);  // ← Отрицательная сторона
    axisZ[0].color = sf::Color(100, 100, 200);     // Чуть темнее
    axisZ[1].position = project(0, 0, axis_len);   // ← Положительная сторона
    axisZ[1].color = sf::Color::Blue;
    window.draw(axisZ);

    // 4. Рисуем основные синусоиды
    window.draw(vertices_E);
    window.draw(vertices_B);
}
