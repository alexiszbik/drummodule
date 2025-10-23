#pragma once
#include "DrumBase.h"
#include "Decay.h"

using namespace daisysp;

class Rim : public DrumBase {
public:
    Rim() : DrumBase() {
    }

    void Init(float sampleRate) override {
        for (int i = 0; i < oscCount; i++) {
            oscs[i].Init(sampleRate);
            oscs[i].SetWaveform(Oscillator::WAVE_SQUARE);
            oscs[i].SetAmp(1.f);
        }

        mtof(60);
        oscs[0].SetFreq(455.f);
        oscs[1].SetFreq(1667.f);
 
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
        float out = (oscs[0].Process() + oscs[1].Process()) * ampEnv.Process() * velocity;
        out = lp.Process(out);
        return hp.Process(out);
    }

    void Trig(float velocity) override {
        for (int i = 0; i < oscCount; i++) {
            oscs[i].Reset();
        }
        this->velocity = velocity;
        ampEnv.Trig();
    }
private:
    static const int oscCount = 2;
    Oscillator oscs[oscCount]; 
    Decay ampEnv;
    Tone lp;
    ATone hp;

    float velocity = 1.f;
};