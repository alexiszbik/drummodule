#pragma once

#include "DrumBase.h"
#include "Decay.h"
#include "BiquadFilter.h"

using namespace daisysp;

//TODO : velocity 

class Cowbell : public DrumBase {
public:
    Cowbell() : DrumBase() {
    }

    void Init(float sampleRate) override {
        for (int i = 0; i < oscCount; i++) {
            oscs[i].Init(sampleRate);
            oscs[i].SetAmp(1);
            oscs[i].SetWaveform(Oscillator::WAVE_SQUARE);
            oscs[i].SetFreq(oscFreqs[i]);
        }

        bp.SetBandpass(880.0f, 6.0f, sampleRate);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(0.12f);

    }

    float Process() override {
        float out = 0;
        for (int i = 0; i < oscCount; i++) {
            out += oscs[i].Process();
        }
        //out *= 3.981;
        //out = DSY_CLAMP(out, -1.f, 1.f);
        out = bp.Process(out);
        float amp = ampEnv.Process();
        return out * amp * amp * velocity * 0.9;
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

    float oscFreqs[oscCount] = {800,540,522.4,304.4,369.6,205.3}; // 2 first frequency should be tunable
    Oscillator oscs[oscCount];
    Decay ampEnv;

    float velocity;

    BiquadFilter bp;
};