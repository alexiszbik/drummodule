#pragma once
#include "DrumBase.h"
#include "Decay.h"

using namespace daisysp;

class Clap : public DrumBase {
public:
    Clap() : DrumBase() {
    }

    void Init(float sampleRate) override {
        noise.Init();
        
        //3sawtooth, 10ms per sawtooth
        decayInc = 1.f / (0.01 * sampleRate);

        hp.Init(sampleRate);
        float hpFreq = 200;
        hp.SetFreq(hpFreq);
    }

    float ProcessAmp() {
        if (trigger) {
            isRunning = true;
            trigger = false;
            ampValue = 3.f; //3sawtooth
        }

        if (isRunning) {
            if (ampValue > 0) {
                ampValue = ampValue - decayInc;
                if (ampValue <= 0) {
                    isRunning = false;
                }
                float modulo = fmodf(ampValue, 1.0f);
                return fmaxf(modulo * modulo, 0);
            } else {
                return 0;
            }
        }
        return 0;
    }

    float Process() override {
        float out = noise.Process() * ProcessAmp() * velocity;
        return hp.Process(out);
    }

    void Trig(float velocity) override {
        trigger = true;
        this->velocity = velocity;
    }

private:
    WhiteNoise noise; 

    ATone hp;
    float velocity = 1.f;

    float ampValue = 0;
    bool trigger = false;
    bool isRunning = false;

    float decayInc = 0.00001f;
};