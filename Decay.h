#pragma once
#include "math.h"

class Decay {
public:
    void Init(float sampleRate) {
        this->sampleRate = sampleRate;
    }

    void SetTime(float time) {
        time = fmaxf(time, 0.000001f);
        increment = 1.f / (time * sampleRate);
    }

    float Process() {
        if (trig) {
            trig = false;
            value = 1.0f;
        }
        if (value > 0) {
            value = value - increment;
            return fmaxf(value * value, 0);
        } else {
            return 0;
        }
    }

    void Trig() {
        trig = true;    
    }
private:
    float sampleRate = 44100;
    float value = 0.f; 
    bool trig = false;
    float increment = 0.001;
};