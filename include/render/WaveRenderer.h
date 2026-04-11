// include/render/WaveRenderer.h
#pragma once

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
    sf::Vector2f offset;


    sf::Vector2f project(float x, float y, float z) const;


    void updateVertices();

public:

    WaveRenderer(const EMWave& em_wave,
                 float scale = 1.0f,
                 int samples = 200,
                 float z_range = 400.0f);


    void update();


    void draw(sf::RenderWindow& window) const;


    void setColors(sf::Color e, sf::Color b) { color_E = e; color_B = b; }
    void setScale(float s) { scale = s; updateVertices(); }
    void setCameraAngle(float angle) { cam_angle = angle; updateVertices(); }
    void setOffset(sf::Vector2f o) { offset = o; updateVertices(); }
};
