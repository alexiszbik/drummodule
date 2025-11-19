#pragma once
#include "DrumBase.h"
#include "Decay.h"

using namespace daisysp;

class Snare : public DrumBase {
public:
    Snare() : DrumBase() {
    }

    void Init(float sampleRate) override {
        noise.Init();

        osc.Init(sampleRate);
        osc.SetWaveform(osc.WAVE_SIN);
        osc.SetAmp(1.f);

        pitchEnv.Init(sampleRate);
        pitchEnv.SetTime(0.02);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(0.13);

        hp.Init(sampleRate);
        float hpFreq = 200;
        hp.SetFreq(hpFreq);
    }

    float Process() override {
        int pitch = basePitch + pitchEnv.Process() * 29;
        osc.SetFreq(mtof(pitch));
        float out = (osc.Process() + noise.Process()) * ampEnv.Process() * velocity;
        return hp.Process(out);
    }

    void Trig(float velocity) override {
        osc.Reset();
        ampEnv.Trig();
        pitchEnv.Trig();
        this->velocity = velocity;
    }

private:
    Oscillator osc;
    WhiteNoise noise; 
    Decay pitchEnv;
    Decay ampEnv;

    ATone hp;

    float basePitch = 50;
    float velocity = 1.f;
};