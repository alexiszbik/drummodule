#pragma once
#include "DrumBase.h"
#include "Decay.h"

using namespace daisysp;

class Tom : public DrumBase {
public:
    Tom(int pitch) : DrumBase(), basePitch(pitch) {
    }

    void Init(float sampleRate) override {
        osc.Init(sampleRate);
        osc.SetWaveform(osc.WAVE_TRI);
        osc.SetAmp(1.f);

        pitchEnv.Init(sampleRate);
        pitchEnv.SetTime(0.02);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(0.17);
    }

    float Process() override {
        int pitch = basePitch + pitchEnv.Process() * 24;
        osc.SetFreq(mtof(pitch));
        return osc.Process() * ampEnv.Process() * 0.707 * velocity;
    }

    void Trig(float velocity) override {
        osc.Reset();
        osc.SetAmp(velocity);
        ampEnv.Trig();
        pitchEnv.Trig();
        this->velocity = velocity;
    }

private:
    Oscillator osc; 
    Decay pitchEnv;
    Decay ampEnv;

    float basePitch = 48;
    float velocity = 1.0;
};