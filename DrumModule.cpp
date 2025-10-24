#include "daisy_seed.h"
#include "daisysp.h"

#include "Tom.h"
#include "SinPerc.h"
#include "Rim.h"
#include "Kick.h"
#include "Shaker.h"
#include "Snare.h"
#include "Clap.h"
#include "Hat.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed  hardware;

static const int baseNote = 36;

DrumBase* drums[] = {new Kick(), new Snare(), new Rim(), new Clap(), new Hat(), new Hat(true), new Shaker(), new Tom(40), new Tom(44), new Tom(48), new Tom(52), new SinPerc(64), new SinPerc(60), new SinPerc(96)};

//Missing : hat hh, hh open, ride ?, cowbell ?

int drumCount = sizeof(drums) / sizeof(drums[0]);

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

        float osc_out = 0;
        for (int i = 0; i < drumCount; i++) {
            osc_out += drums[i]->Process();
        }

        osc_out *= 0.707;
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
        bool gate = noteOn.velocity > 0;
        
        if (gate) {
            int pitch = noteOn.note;
            
            if (pitch >= baseNote && pitch < (baseNote + drumCount)) {
                float velocity = noteOn.velocity / 127.f;
                velocity *= velocity;
                drums[pitch - baseNote]->Trig(velocity);
            }
        }
    }
    else if (m.type == NoteOff) {
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
    float sampleRate = hardware.AudioSampleRate();

    for (int i = 0; i < drumCount; i++) {
        drums[i]->Init(sampleRate);
    }
    
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
