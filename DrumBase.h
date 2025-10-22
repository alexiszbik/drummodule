#pragma once
#include "daisysp.h"

using namespace daisysp;

class DrumBase {
public:
    virtual void Init(float sampleRate) = 0;

    virtual float Process() = 0;

    virtual void Trig(float velocity)  = 0;
};