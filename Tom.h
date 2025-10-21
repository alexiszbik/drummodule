#pragma once
#include "daisysp.h"
#include "Decay.h"

using namespace daisysp;

class Tom {
public:
    void Init(float sampleRate) {
        osc.Init(sampleRate);
        osc.SetWaveform(osc.WAVE_SIN);
        osc.SetAmp(1.f);
        osc.SetFreq(440);

        pitchEnv.Init(sampleRate);
        pitchEnv.SetTime(0.02);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(0.2);
    }

    float Process() {
        int pitch = basePitch + pitchEnv.Process() * 24;
        osc.SetFreq(mtof(pitch));
        return osc.Process() * ampEnv.Process();
    }

    void Trig() {
        ampEnv.Trig();
        pitchEnv.Trig();
    }
private:
    Oscillator osc; 
    Decay pitchEnv;
    Decay ampEnv;

    float basePitch = 48;
};