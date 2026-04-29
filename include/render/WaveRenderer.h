// include/render/WaveRenderer.h
#pragma once
#include <algorithm>

#include <SFML/Graphics.hpp>
#include "../physics/EMWave.h"

class WaveRenderer {
private:
    const EMWave& wave;

    sf::Color color_E;
    sf::Color color_B;
    float scale;
    int samples;
    float z_range;



    sf::VertexArray vertices_E;
    sf::VertexArray vertices_B;


    float cam_angle;
    float yaw;
    float pitch;
    sf::Vector2f offset;


    sf::Vector2f project(float x, float y, float z) const;
    void updateVertices();

public:

    WaveRenderer(const EMWave& em_wave,
                 float scale = 1.0f,
                 int samples = 300,
                 float z_range = 600.0f);


    void update();
    void draw(sf::RenderWindow& window) const;

    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    void setYaw(float y) { yaw = y; updateVertices(); }
    void setPitch(float p) {
        pitch = std::max(-1.5f, std::min(1.5f, p));
        updateVertices();
    }

    void setColors(sf::Color e, sf::Color b) { color_E = e; color_B = b; }
    void setScale(float s) { scale = s; updateVertices(); }
    void setOffset(sf::Vector2f o) { offset = o; updateVertices(); }
};
