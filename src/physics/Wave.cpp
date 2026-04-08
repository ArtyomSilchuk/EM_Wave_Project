#include "physics/EMWave.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415926
#endif // M_PI


void EMWave::recalc() {
    angular_freq = 2.0f * M_PI * frequency;
    wave_number = 2.0f * M_PI / wavelength;
    speed = frequency * wavelength;
}


EMWave::EMWave(float freq, float lambda, float amp_E, float amp_B, float phi)
    :frequency(freq), wavelength(lambda), amplitude_E(amp_E), amplitude_B(amp_B), phase(phi), time(0.0f)

{
    recalc();
}

float EMWave::calculateE(float t, float z) const {
    float argument = angular_freq * t - wave_number * z + phase;
    return amplitude_E * std::sin(argument);
}

float EMWave::calculateB(float t, float z) const {
    float argument = angular_freq * t - wave_number * z + phase;
    return amplitude_B * std::sin(argument);
}

void EMWave::update(float dt) {
    time += dt;
}

void EMWave::setFrequency(float f) {
    if (f > 0.0f) {
        frequency = f;
        recalc();
    }
}

void EMWave::setWavelength(float l) {
    if (l > 0.0f) {
        wavelength = l;
        recalc();
    }
}

void EMWave::setAmplitudeE(float a) {
    if (a > 0.0f) {
        amplitude_E = a;
    }
}

void EMWave::setAmplitudeB(float a) {
    if (a > 0.0f) {
        amplitude_B = a;
    }
}

void EMWave::setPhase(float p) {
    phase = p;
}
