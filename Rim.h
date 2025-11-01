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
        ampEnv.SetTime(0.008f);

        lp.Init(sampleRate);
        hp.Init(sampleRate);

        float lpFreq = 4500.f;
        lp.SetFreq(lpFreq);
        float hpFreq = 600.f;
        hp.SetFreq(hpFreq);

    }

    float Process() override {
        float out = oscBank.Process() * ampEnv.Process() * velocity;
        out = lp.Process(out);
        return hp.Process(out);
    }

    void Trig(float velocity) override {
        oscBank.Reset();
        this->velocity = velocity;
        ampEnv.Trig();
    }
private:
    static const int oscCount = 2;
    OscBank oscBank = OscBank({455, 1667});
    Decay ampEnv;
    Tone lp;
    ATone hp;

    float velocity = 1.f;
};