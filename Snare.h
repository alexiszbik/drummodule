#pragma once

#include "DrumBase.h"

using namespace daisysp;

//TODO : velocity

class Snare : public DrumBase {
public:
    Snare() : DrumBase() {
    }

    void Init(float sampleRate) override {
        sd.Init(sampleRate);

        sd.SetSnappy(0.5f);
        sd.SetAccent(0.5);
        sd.SetDecay(0.5 );
    }

    float Process() override {
        return sd.Process();
    }

    void Trig(float velocity)  override {
        sd.Trig();
    }
private:
    SyntheticSnareDrum sd;
};