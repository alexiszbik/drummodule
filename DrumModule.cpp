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
#include "Cowbell.h"
#include "Ride.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisysp;

// Declare a DaisySeed object called hardware
DaisySeed  hardware;

static const int baseNote = 36;
 
//Declare all drums
Kick kick;
Snare snare;
Rim rim;
Clap clap;
Hat hat;
Shaker shaker;
Tom tom40(40);
Tom tom44(44);
Tom tom48(48);
Tom tom52(52);
SinPerc sin64(64);
SinPerc sin60(60);
SinPerc sin96(96);
Cowbell cowbell;
Ride ride;

//Put all drums in array
DrumBase* drums[] = {
    &kick,
    &snare,
    &rim,
    &clap,
    &hat,
    &shaker,
    &tom40,
    &tom44,
    &tom48,
    &tom52,
    &sin64,
    &sin60,
    &sin96,
    &cowbell,
    &ride
};

//Map for midi notes
DrumBase* drumMap[] = {
    &kick,
    &snare,
    &rim,
    &clap,
    &hat,
    &hat,
    &shaker,
    &tom40,
    &tom44,
    &tom48,
    &tom52,
    &sin64,
    &sin60,
    &sin96,
    &cowbell,
    &ride
};

const short openHatIndex = 5; //Hardcoded, but hey ...

//Missing : hat hh, hh open, ride ?, cowbell ?

int drumCount = sizeof(drums) / sizeof(drums[0]);
int drumMapCount = sizeof(drumMap) / sizeof(drumMap[0]);

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
        osc_out = SoftClip(osc_out);
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
            
            if (pitch >= baseNote && pitch < (baseNote + drumMapCount)) {
                float velocity = noteOn.velocity / 127.f;
                velocity *= velocity;
                
                if (pitch - baseNote == openHatIndex - 1) {
                    hat.setIsOpen(false);
                }
                if (pitch - baseNote == openHatIndex) {
                    hat.setIsOpen(true);
                }
                drumMap[pitch - baseNote]->Trig(velocity);
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
    //midi.StartReceive();
    
    for(;;)
    {
        midi.Listen();
        // Handle MIDI Events
        while(midi.HasEvents())
        {
            HandleMidiMessage(midi.PopEvent());
        }
        //System::Delay(1);
    };
}
