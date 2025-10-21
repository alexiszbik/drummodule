#include "daisy_seed.h"
#include "daisysp.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed  hardware;
Oscillator osc;
AdEnv      env;

MidiUartHandler midi;

Switch button1;

int note = 60;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{

    //Fill the block with samples
    for(size_t i = 0; i < size; i += 2)
    {
        //Get the next envelope value
        //get the next oscillator sample
        float osc_out = osc.Process();

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
        note = noteOn.note;
        osc.SetFreq(mtof(note));
        osc.SetAmp(noteOn.velocity > 0);
    }
    else if (m.type == NoteOff) {
        osc.SetAmp(0);
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
