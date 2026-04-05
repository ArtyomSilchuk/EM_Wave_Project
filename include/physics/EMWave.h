#pragma once

class EMWave {
private:
    float frequency;
    float wavelength;
    float amplitude_E;
    float amplitude_B;
    float phase;

    float angular_freq;
    float wave_number;
    float speed;

    float time;

    void recalc();

public:
    EMWave(float freq = 1.0f, float lambda = 1.0f,
            float amp_E = 1.0f, float amp_B = 1.0f, float phi = 0.0f);

    float calculateE(float t, float z) const;
    float calculateB(float t, float z) const;

    void update(float dt);

    float getFrequency() const { return frequency; }
    float getWavelength() const { return wavelength; }
    float getAmplitudeE() const { return amplitude_E; }
    float getAmplitudeB() const { return amplitude_B; }
    float getPhase() const { return phase; }
    float getTime() const { return time; }

    void setFrequency(float f);
    void setWavelength(float l);
    void setAmplitudeE(float a);
    void setAmplitudeB(float a);
    void setPhase(float p);

    void resetTime() { time = 0.0f; }
};
