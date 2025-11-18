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
        ampEnv.SetTime(0.012f);

        hp.Init(sampleRate);

        float hpFreq = 200.f;
        hp.SetFreq(hpFreq);

    }

    float Process() override {
        float out = oscBank.Process() * ampEnv.Process() * velocity * 1;
        //out = lp.Process(out);
        return hp.Process(out);
    }

    void Trig(float velocity) override {
        oscBank.Reset(0.5);
        this->velocity = velocity;
        ampEnv.Trig();
    }
private:
    OscBank oscBank = OscBank({455, 1667});
    Decay ampEnv;
    ATone hp;

    float velocity = 1.f;
};