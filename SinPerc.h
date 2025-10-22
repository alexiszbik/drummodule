#pragma once
#include "DrumBase.h"
#include "Decay.h"

using namespace daisysp;

class SinPerc : public DrumBase {
public:
    SinPerc(int pitch) : DrumBase(), freq(mtof(pitch)) {
    }

    void Init(float sampleRate) override {
        osc.Init(sampleRate);
        osc.SetWaveform(osc.WAVE_SIN);
        osc.SetAmp(1.f);
        osc.SetFreq(freq);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(0.05);
    }

    float Process() override {
        return osc.Process() * ampEnv.Process();
    }

    void Trig(float velocity)  override {
        osc.Reset();
        osc.SetAmp(velocity);
        ampEnv.Trig();
    }
private:
    Oscillator osc; 
    Decay ampEnv;

    float freq = 440;
};