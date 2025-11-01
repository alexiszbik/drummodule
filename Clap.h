#pragma once
#include "DrumBase.h"
#include "Decay.h"

using namespace daisysp;

class Clap : public DrumBase {
private:
enum AmpState {
    Idle = 0,
    Sawtooth,
    Release
};

public:
    Clap() : DrumBase() {
    }

    void Init(float sampleRate) override {
        noise.Init();
        
        //3 sawtooth, 10ms per sawtooth
        sawInc = 1.f / (0.01 * sampleRate);
        releaseInc = 1.f / (0.12 * sampleRate);

        hp.Init(sampleRate);
        float hpFreq = 1000;
        hp.SetFreq(hpFreq);

        lp.Init(sampleRate);
        float lpFreq = 3600;
        lp.SetFreq(lpFreq);
    }

    float ProcessAmp() {
        if (trigger) {
            state = Sawtooth;
            trigger = false;
            ampValue = 3.f; // 3sawtooth
        }
        if (state == Sawtooth) {
            ampValue = ampValue - sawInc;
            float modulo = fmodf(ampValue, 1.0f);
            if (ampValue <= 0) {
                state = Release;
                ampValue = 1.f;
            }
            return fmaxf(modulo * modulo * modulo, 0);
        }
        if (state == Release) {
            ampValue = ampValue - releaseInc;
            if (ampValue <= 0) {
                state = Idle;
            }
            return fmaxf(ampValue * ampValue * ampValue, 0);
            //TODO the release
        }

        return 0;
    }

    float Process() override {
        float out = noise.Process();
        out = hp.Process(out);
        return lp.Process(out) * ProcessAmp() * velocity * 1.9f; // ++3dB ??
    }

    void Trig(float velocity) override {
        trigger = true;
        this->velocity = velocity;
    }

private:
    WhiteNoise noise; 

    ATone hp;
    Tone lp;
    float velocity = 1.f;

    float ampValue = 0;
    bool trigger = false;
    AmpState state = Idle;

    float sawInc = 0.00001f;
    float releaseInc = 0.00001f;
};