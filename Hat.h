#pragma once

#include "DrumBase.h"

using namespace daisysp;

//TODO : velocity

class Hat : public DrumBase {
public:
    Hat() : DrumBase() {
    }

    void Init(float sampleRate) override {
        hh.Init(sampleRate);

        hh.SetDecay(1.f);
        hh.SetSustain(false);
        hh.SetNoisiness(0.6f);
        hh.SetTone(0.5f);
        hh.SetAccent(0.5f);
    }

    float Process() override {
        return hh.Process();
    }

    void Trig(float velocity)  override {
        hh.Trig();
    }
private:
    HiHat<> hh;
};