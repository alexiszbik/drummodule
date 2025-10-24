#pragma once

#include "DrumBase.h"
#include "Decay.h"

using namespace daisysp;

//TODO : velocity

struct BiquadFilter {
    float a0, a1, a2, b0, b1, b2;
    float z1 = 0.0f, z2 = 0.0f;

    void SetBandpass(float centerFreq, float q, float sampleRate) {
        float w0 = 2.0f * PI_F * centerFreq / sampleRate;
        float alpha = sin(w0) / (2.0f * q);

        float cosw0 = cos(w0);
        b0 =   alpha;
        b1 =   0.0f;
        b2 =  -alpha;
        a0 =   1.0f + alpha;
        a1 =  -2.0f * cosw0;
        a2 =   1.0f - alpha;

        // Normalisation
        b0 /= a0;
        b1 /= a0;
        b2 /= a0;
        a1 /= a0;
        a2 /= a0;
    }

    float Process(float in) {
        float out = b0 * in + z1;
        z1 = b1 * in - a1 * out + z2;
        z2 = b2 * in - a2 * out;
        return out;
    }
};

class Hat : public DrumBase {
public:
    Hat(bool isOpen = false) : DrumBase(), isOpen(isOpen) {
    }

    void Init(float sampleRate) override {
        for (int i = 0; i < oscCount; i++) {
            oscs[i].Init(sampleRate);
            oscs[i].SetAmp(1);
            oscs[i].SetWaveform(Oscillator::WAVE_SQUARE);
            oscs[i].SetFreq(oscFreqs[i]);
        }

        bp.SetBandpass(7000.0f, 8.0f, sampleRate);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(isOpen ? 0.6 : 0.09f);

        hp.Init(sampleRate);

        float hpFreq = 10000.f;
        hp.SetFreq(hpFreq);
    }

    float Process() override {
        float out = 0;
        for (int i = 0; i < oscCount; i++) {
            out += oscs[i].Process();
        }
        out = bp.Process(out);
        out = hp.Process(out);
        return out * ampEnv.Process() * velocity * 3.981;
    }

    void Trig(float velocity)  override {
        this->velocity = velocity;
        ampEnv.Trig();
        for (int i = 0; i < oscCount; i++) {
            oscs[i].Reset();
        }
    }
private:
    static const int oscCount = 6;

    float oscFreqs[oscCount] = {800,540,523,370,304,205};
    Oscillator oscs[oscCount];
    Decay ampEnv;

    float velocity;

    ATone hp;

    BiquadFilter bp;

    bool isOpen = false;
    
};