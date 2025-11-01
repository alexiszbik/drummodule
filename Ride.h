#pragma once

#include "DrumBase.h"
#include "Decay.h"
#include "BiquadFilter.h"

using namespace daisysp;

//TODO : velocity 

class Ride : public DrumBase {
public:
    Ride() : DrumBase() {
    }

    void Init(float sampleRate) override {
        oscBank.Init(sampleRate);
        bp.SetBandpass(7000.0f, 8.0f, sampleRate);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(2);

        hp.Init(sampleRate);

        float hpFreq = 12000.f;
        hp.SetFreq(hpFreq);
    }

    float Process() override {
        float out = oscBank.Process();
        out = bp.Process(out);
        out = hp.Process(out);
        float amp = ampEnv.Process();
        return out * amp * amp * velocity * 3.981;
    }

    void Trig(float velocity)  override {
        this->velocity = velocity;
        ampEnv.Trig();
        oscBank.Reset();
    }
private:
    OscBank oscBank = OscBank({1200,933,523,370,304,205});
    Decay ampEnv;

    float velocity;

    ATone hp;

    BiquadFilter bp;

};