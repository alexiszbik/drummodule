#pragma once

#include "DrumBase.h"
#include "Decay.h"
#include "BiquadFilter.h"
#include "OscBank.h"

using namespace daisysp;

//TODO : velocity 

class Hat : public DrumBase {
public:
    Hat(bool isOpen = false) : DrumBase(), isOpen(isOpen) {
    }

    void Init(float sampleRate) override {
        oscBank.Init(sampleRate);

        bp.SetBandpass(7000.0f, 8.0f, sampleRate);

        ampEnv.Init(sampleRate);
        ampEnv.SetTime(isOpen ? 0.6 : 0.09f);

        hp.Init(sampleRate);

        float hpFreq = 10000.f;
        hp.SetFreq(hpFreq);
    }

    float Process() override {
        float out = oscBank.Process();
        out = bp.Process(out);
        out = hp.Process(out);
        return out * ampEnv.Process() * velocity * 3.981;
    }

    void Trig(float velocity)  override {
        this->velocity = velocity;
        ampEnv.Trig();
        oscBank.Reset();
    }

private:
    OscBank oscBank = OscBank({800,540,523,370,304,205});

    Decay ampEnv;

    float velocity;

    ATone hp;

    BiquadFilter bp;

    bool isOpen = false;
};