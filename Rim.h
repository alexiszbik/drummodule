#pragma once
#include "DrumBase.h"
#include "OscBank.h"
#include "Decay.h"

using namespace daisysp;

class Rim : public DrumBase {
public:
    Rim() : DrumBase() {
    }

    void Init(float sampleRate) override {
        oscBank.Init(sampleRate);
 
        ampEnv.Init(sampleRate);
        ampEnv.SetTime(0.009f);

        hp.Init(sampleRate);

        float hpFreq = 650.f;
        hp.SetFreq(hpFreq);

    }

    float Process() override {
        float out = oscBank.Process() * ampEnv.Process() * velocity ;
        out = tanhf(out * 2.2f) * 0.707;
        return hp.Process(out);
    }

    void Trig(float velocity) override {
        oscBank.Reset(0.5);
        this->velocity = velocity;
        ampEnv.Trig();
    }
private:
    OscBank oscBank = OscBank({368.f, 540.f, 830.f});
    Decay ampEnv;
    ATone hp;

    float velocity = 1.f;
};