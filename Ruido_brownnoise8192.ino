

/*  Example of filtering 2 waves with different filter settings,
    using Mozzi sonification library.

    Demonstrates 2 Oscillators, each with a LowPassFilter().

    Circuit: Audio output on digital pin 9 on a Uno or similar, or
    DAC/A14 on Teensy 3.1, or
    check the README or http://sensorium.github.io/Mozzi/

   Mozzi documentation/API
   https://sensorium.github.io/Mozzi/doc/html/index.html

   Mozzi help/discussion/announcements:
   https://groups.google.com/forum/#!forum/mozzi-users

   Copyright 2012-2024 Tim Barrass and the Mozzi Team

   Mozzi is licensed under the GNU Lesser General Public Licence (LGPL) Version 2.1 or later.
*/

#include <Mozzi.h>
#include <Oscil.h>
#include <tables/brownnoise8192_int8.h>
//#include <tables/chum9_int8.h> // recorded audio wavetable
#include <tables/cos512_int8.h> // for filter modulation
#include <ResonantFilter.h>
#include <mozzi_rand.h> // for rand()


Oscil <BROWNNOISE8192_NUM_CELLS, MOZZI_AUDIO_RATE> aNoise(BROWNNOISE8192_DATA);
Oscil <BROWNNOISE8192_NUM_CELLS, MOZZI_AUDIO_RATE> aNoise2 (BROWNNOISE8192_DATA);
Oscil<COS512_NUM_CELLS, MOZZI_CONTROL_RATE> kFilterMod1(COS512_DATA); // to modulate filter frequency
Oscil<COS512_NUM_CELLS, MOZZI_CONTROL_RATE> kFilterMod2(COS512_DATA); // to modulate filter frequency

LowPassFilter lpf1;  // can be changed to HighPassFilter, BandPassFilter or NotchFilter
LowPassFilter lpf2;

uint8_t resonance1 = 100; // range 0-255, 255 is most resonant
uint8_t resonance2 = 20;

void setup(){
  startMozzi();
  randSeed();
  aNoise.setFreq((float)MOZZI_AUDIO_RATE/BROWNNOISE8192_SAMPLERATE);
  aNoise2.setFreq((float)MOZZI_AUDIO_RATE/BROWNNOISE8192_SAMPLERATE);
  kFilterMod1.setFreq(0.3f);
  kFilterMod2.setFreq(0.1f);
}


void updateControl(){
  aNoise.setPhase(rand((unsigned int)BROWNNOISE8192_NUM_CELLS));
  aNoise2.setPhase(rand((unsigned int)BROWNNOISE8192_NUM_CELLS));
  // map the modulation depth into the filter range, 0-255 to represent 0-8192 Hz
  byte cutoff_freq1 = 100 + kFilterMod1.next()/2; // 100 ± 63
  lpf1.setCutoffFreqAndResonance(cutoff_freq1, resonance1);
  // also update lpf2 cutoff
  byte cutoff_freq2 = 80 + kFilterMod2.next()/4; // 70 ± 31
  lpf2.setCutoffFreqAndResonance(cutoff_freq2, resonance2);
}


AudioOutput updateAudio(){
  return MonoOutput::fromAlmostNBit(9, (((char)lpf1.next(aNoise.next()))>>1) + (char)lpf2.next(aNoise2.next()));
}

void loop () {
  audioHook();
}