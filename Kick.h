#pragma once
#include "DrumBase.h"
#include "Decay.h"

using namespace daisysp;

class Kick : public DrumBase {
public:
    Kick() : DrumBase() {
    }

    void Init(float sampleRate) override {
        osc.Init(sampleRate);
        osc.SetWaveform(osc.WAVE_SIN);
        osc.SetAmp(1.f);

        pitchEnv.Init(sampleRate);
        pitchEnv.SetTime(0.02);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(0.4);
    }

    float Process() override {
        int pitch = basePitch + pitchEnv.Process() * 40;
        osc.SetFreq(mtof(pitch));
        return osc.Process() * ampEnv.Process();
    }

    void Trig(float velocity) override {
        osc.Reset(0.25f);
        osc.SetAmp(velocity);
        ampEnv.Trig();
        pitchEnv.Trig();
    }
private:
    Oscillator osc; 
    Decay pitchEnv;
    Decay ampEnv;

    float basePitch = 29;
};