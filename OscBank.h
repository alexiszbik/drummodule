#pragma once

#include "DrumBase.h"

using namespace daisysp;
using namespace std;

class OscBank : ProcessBase {

public:
    OscBank(std::vector<float> frequencies) {
        for (auto& freq : frequencies) {
            auto osc = new Oscillator();
            osc->SetAmp(1);
            osc->SetWaveform(Oscillator::WAVE_SQUARE);
            osc->SetFreq(freq);
            oscs.push_back(osc);
        }
    }

    void Init(float sampleRate) override {
        for (auto& osc : oscs) {
            osc->Init(sampleRate);
        }
    }

    float Process() override {
        float out = 0;
        for (auto& osc : oscs) {
            out += osc->Process();
        }
        return out;
    }

    void Reset() {
        for (auto& osc : oscs) {
            osc->Reset();
        }
    }

private:
    std::vector<Oscillator*> oscs;
};