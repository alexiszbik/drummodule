#pragma once
#include "math.h"

class AttackDecay {
private:
enum ADStatus {
    Idle = 0,
    Attack, 
    Decay
};

public:
    void Init(float sampleRate) {
        this->sampleRate = sampleRate;
    }

    void SetAttack(float time) {
        time = fmaxf(time, 0.000001f);
        attackIncrement = 1.f / (time * sampleRate);
    }

    void SetDecay(float time) {
        time = fmaxf(time, 0.000001f);
        decayIncrement = 1.f / (time * sampleRate);
    }

    float Process() {
        if (trig) {
            trig = false;
            value = 0.0f;
            status = Attack;
        }
        if (status == Attack) {
            value = value + decayIncrement;
            if (value >= 1) {
                value = 1;
                status = Decay;
            }
            return value * value;
        }
        if (status == Decay) {
            if (value > 0) {
                value = value - decayIncrement;
                if (value <= 0) {
                    status = Idle;
                    value = 0;
                }
                return value * value;
            }
        }
        return 0;
    }

    void Trig() {
        trig = true;    
    }
private:
    float sampleRate = 44100;
    float value = 0.f; 
    bool trig = false;
    float decayIncrement = 0.001;
    float attackIncrement = 0.001;

    ADStatus status = Idle;
};