#pragma once

#include "DrumBase.h"
#include "Decay.h"
#include "BiquadFilter.h"

using namespace daisysp;

//TODO : velocity 

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
        //out *= 3.981;
        //out = DSY_CLAMP(out, -1.f, 1.f);
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

    float oscFreqs[oscCount] = {800,540,523,370,304,205}; // 2 first frequency should be tunable
    Oscillator oscs[oscCount];
    Decay ampEnv;

    float velocity;

    ATone hp;

    BiquadFilter bp;

    bool isOpen = false;
};