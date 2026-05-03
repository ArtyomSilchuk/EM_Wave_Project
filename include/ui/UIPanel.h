#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "physics/EMWave.h"

class UIPanel {
public:
    UIPanel(sf::RenderWindow& window, const sf::Font& font);
    void draw();
    void handleEvent(const sf::Event& event, EMWave& wave);
    void updateInfo(const EMWave& wave); // Обновляет только текстовые данные

private:
    sf::RenderWindow& window;
    const sf::Font& font;
    sf::RectangleShape bg;
    sf::Text title;
    std::vector<sf::Text> infoLabels;
    std::vector<sf::Text> infoValues;

    struct Slider {
        std::string labelStr;
        sf::Text label;
        sf::RectangleShape track;
        sf::CircleShape thumb;
        sf::Text valText;
        float minVal, maxVal, curVal;
        bool isHovered(sf::Vector2f mp) const { 
            return track.getGlobalBounds().contains(mp) || thumb.getGlobalBounds().contains(mp); 
        }
    };
    std::vector<Slider> sliders;
    int dragIdx = -1;

    void layout();
    void updateThumbPos(Slider& s);
    void applyToWave(EMWave& wave, int idx);
};
