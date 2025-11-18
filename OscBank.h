#pragma once

#include "DrumBase.h"

using namespace daisysp;
using namespace std;

class OscBank : ProcessBase {

public:
    OscBank(std::vector<float> frequencies) : frequencies(frequencies) {
        
    }

    void Init(float sampleRate) override {
        for (auto& freq : frequencies) {
            auto osc = new Oscillator();
            osc->Init(sampleRate);
            osc->SetAmp(1);
            osc->SetWaveform(Oscillator::WAVE_SQUARE);
            osc->SetFreq(freq);
            oscs.push_back(osc);
        }
    }

    float Process() override {
        float out = 0;
        for (auto& osc : oscs) {
            out += osc->Process();
        }
        return out;
    }

    void Reset(float phase = 0 ) {
        for (auto& osc : oscs) {
            osc->Reset(phase);
        }
    }

private:
    std::vector<float> frequencies; 
    std::vector<Oscillator*> oscs;
};