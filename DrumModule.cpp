#include "daisy_seed.h"
#include "daisysp.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed  hardware;
Oscillator osc;

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
            return value * value;
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

bool gate = false;
float basePitch = 60;

Decay pitchEnv;
Decay ampEnv;

MidiUartHandler midi;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{

    //Fill the block with samples
    for(size_t i = 0; i < size; i += 2)
    {
        //Get the next envelope value
        //get the next oscillator sample
    
        int pitch = basePitch + pitchEnv.Process() * 24;

        osc.SetFreq(mtof(pitch));

        float osc_out = osc.Process() * ampEnv.Process();

        //Set the left and right outputs
        out[i]     = osc_out;
        out[i + 1] = osc_out;
    }
}


// Typical Switch case for Message Type.
void HandleMidiMessage(MidiEvent m)
{
    if (m.type == NoteOn) {
        NoteOnEvent noteOn = m.AsNoteOn();
        basePitch = noteOn.note;
        gate = noteOn.velocity > 0;
        if (gate) {
            ampEnv.Trig();
            pitchEnv.Trig();
        }
    }
    else if (m.type == NoteOff) {
         gate = false;
    } 
}

void InitMidi()
{
    MidiUartHandler::Config midi_config;
    midi.Init(midi_config);
}


int main(void)
{
    // Configure and Initialize the Daisy Seed
    // These are separate to allow reconfiguration of any of the internal
    // components before initialization.
    hardware.Configure();
    hardware.Init();

    InitMidi();

    //How many samples we'll output per second
    float samplerate = hardware.AudioSampleRate();

    //Set up oscillator
    osc.Init(samplerate);
    osc.SetWaveform(osc.WAVE_SIN);
    osc.SetAmp(1.f);
    osc.SetFreq(440);

    //Start the adc
    //hardware.adc.Start();
    //Good settings for a TOM :)
    pitchEnv.Init(samplerate);
    pitchEnv.SetTime(0.02);

    ampEnv.Init(samplerate);
    ampEnv.SetTime(0.2);

    //Start calling the audio callback
    hardware.StartAudio(AudioCallback);

    // Loop forever
    
    for(;;)
    {
        midi.Listen();
        // Handle MIDI Events
        while(midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }
    };
}
