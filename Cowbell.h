#pragma once

#include "DrumBase.h"
#include "Decay.h"
#include "BiquadFilter.h"
#include "OscBank.h"

using namespace daisysp;

//TODO : velocity 

class Cowbell : public DrumBase {
public:
    Cowbell() : DrumBase() {
    }

    void Init(float sampleRate) override {
        oscBank.Init(sampleRate);

        bp.SetBandpass(880.0f, 6.0f, sampleRate);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(0.12f);

    }

    float Process() override {
        float out = oscBank.Process();
        out = bp.Process(out);
        float amp = ampEnv.Process();
        return out * amp * amp * velocity * 0.9;
    }

    void Trig(float velocity)  override {
        this->velocity = velocity;
        ampEnv.Trig();
        oscBank.Reset();
    }
private:
    Decay ampEnv;

    float velocity;

    BiquadFilter bp;
    OscBank oscBank = OscBank({800,540,522.4,304.4,369.6,205.3});
};