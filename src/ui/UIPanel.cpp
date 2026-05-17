#include "ui/UIPanel.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

UIPanel::UIPanel(sf::RenderWindow& window, const sf::Font& font) // Panel
    : window(window), font(font), dragIdx(-1)
{
    bg.setSize({400, 225});
    bg.setPosition(5, 5);
    bg.setFillColor(sf::Color(89, 53, 87));

    title.setFont(font);
    title.setString("EM Wave Parameters");
    title.setCharacterSize(18);
    title.setFillColor(sf::Color::White);
    title.setPosition(120, 10);

    sliders.resize(4);

    // Slider 0: Frequency
    sliders[0].labelStr = "Frequency";
    sliders[0].minVal = 0.1f;
    sliders[0].maxVal = 5.0f;
    sliders[0].curVal = 0.5f;

    // Slider 1: Wavelength
    sliders[1].labelStr = "Wavelength";
    sliders[1].minVal = 2.0f;
    sliders[1].maxVal = 20.0f;
    sliders[1].curVal = 6.0f;

    // Slider 2: Amplitude E
    sliders[2].labelStr = "Amplitude E";
    sliders[2].minVal = 20.0f;
    sliders[2].maxVal = 150.0f;
    sliders[2].curVal = 100.0f;

    // Slider 3: Amplitude B
    sliders[3].labelStr = "Amplitude B";
    sliders[3].minVal = 20.0f;
    sliders[3].maxVal = 150.0f;
    sliders[3].curVal = 100.0f;

    //infoLabels.resize(6);
    //infoValues.resize(6);
    layout();
}

void UIPanel::layout() {
    float y = 35;
    for (auto& s : sliders) {
        s.label.setFont(font);
        s.label.setString(s.labelStr + ":");
        s.label.setCharacterSize(14);
        s.label.setFillColor(sf::Color(200, 200, 200));
        s.label.setPosition(15, y+3);

        s.track.setSize({320, 6});
        s.track.setPosition(15, y + 30);
        s.track.setFillColor(sf::Color(130, 78, 127));

        s.thumb.setSize(sf::Vector2f(10.f, 20.f));
        s.thumb.setFillColor(sf::Color(255, 153, 249));
        s.thumb.setOrigin(5, 10);

        s.valText.setFont(font);
        s.valText.setCharacterSize(14);
        s.valText.setFillColor(sf::Color(200, 200, 200));
        s.valText.setPosition(350, y+22);

        updateThumbPos(s);
        y += 45;
    }
}

void UIPanel::updateThumbPos(Slider& s) {
    float ratio = (s.curVal - s.minVal) / (s.maxVal - s.minVal);
    ratio = std::max(0.0f, std::min(1.0f, ratio)); // Clamp
    float x = s.track.getPosition().x + ratio * s.track.getSize().x;
    s.thumb.setPosition(x, s.track.getPosition().y + 3);
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2) << s.curVal;
    s.valText.setString(ss.str());
}

void UIPanel::applyToWave(EMWave& wave, int idx) {
    switch(idx) {
        case 0: wave.setFrequency(sliders[idx].curVal); break;
        case 1: wave.setWavelength(sliders[idx].curVal); break;
        case 2: wave.setAmplitudeE(sliders[idx].curVal); break;
        case 3: wave.setAmplitudeB(sliders[idx].curVal); break;
    }
}

void UIPanel::handleEvent(const sf::Event& event, EMWave& wave) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        for (int i = 0; i < 4; ++i) {
            if (sliders[i].isHovered(mp)) {
                dragIdx = i;
                float ratio = (mp.x - sliders[i].track.getPosition().x) / sliders[i].track.getSize().x;
                ratio = std::max(0.0f, std::min(1.0f, ratio));
                sliders[i].curVal = sliders[i].minVal + ratio * (sliders[i].maxVal - sliders[i].minVal);
                updateThumbPos(sliders[i]);
                applyToWave(wave, i);
                return;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        dragIdx = -1;
    }
    else if (event.type == sf::Event::MouseMoved && dragIdx != -1) {
        sf::Vector2f mp(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
        float ratio = (mp.x - sliders[dragIdx].track.getPosition().x) / sliders[dragIdx].track.getSize().x;
        ratio = std::max(0.0f, std::min(1.0f, ratio));
        sliders[dragIdx].curVal = sliders[dragIdx].minVal + ratio * (sliders[dragIdx].maxVal - sliders[dragIdx].minVal);
        updateThumbPos(sliders[dragIdx]);
        applyToWave(wave, dragIdx);
    }
}

void UIPanel::draw() {
    window.draw(bg);
    window.draw(title);
    for (const auto& s : sliders) {
        window.draw(s.label);
        window.draw(s.track);
        window.draw(s.thumb);
        window.draw(s.valText);
    }
}
