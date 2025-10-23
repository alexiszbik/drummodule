#pragma once
#include "DrumBase.h"
#include "AttackDecay.h"

using namespace daisysp;

class Shaker : public DrumBase {
public:
    Shaker() : DrumBase() {
    }

    void Init(float sampleRate) override {
        noise.Init();
        hp.Init(sampleRate);

        ampEnv.Init(sampleRate);
        ampEnv.SetAttack(0.008);
        ampEnv.SetDecay(0.03);

        float hpFreq = 3200.f;
        hp.SetFreq(hpFreq);
    }

    float Process() override {
        float out = noise.Process() * ampEnv.Process() * velocity;
        return hp.Process(out);
    }

    void Trig(float velocity) override {
        this->velocity = velocity;
        ampEnv.Trig();
    }
private:
    WhiteNoise noise; 
    AttackDecay ampEnv;
    ATone hp;
    float velocity;
};