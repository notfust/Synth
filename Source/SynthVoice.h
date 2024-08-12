/*
  ==============================================================================

    SynthVoice.h
    Created: 14 May 2023 3:35:58pm
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "OscData.h"
#include "FilterData.h"
#include "AdsrData.h"

using namespace juce;

class SynthVoice
    : public SynthesiserVoice
{
public:
    SynthVoice();
    bool canPlaySound (SynthesiserSound* sound) override;
    void startNote (int               midiNoteNumber,
                    float             velocity,
                    SynthesiserSound* sound,
                    int               currentPitchWheelPosition) override;
    void stopNote (float velocity,
                   bool  allowTailOff) override;
    
    void controllerMoved (int controllerNumber,
                          int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay   (double sampleRate,
                          int    samplesPerBlock,
                          int    outputChannels);
    void renderNextBlock (AudioBuffer< float >& outputBuffer,
                          int                   startSample,
                          int                   numSamples) override;
    
    void reset();
    
    std::array<OscData, 2>& getOscillator1() { return osc1; }
    std::array<OscData, 2>& getOscillator2() { return osc2; }
    
    AdsrData& getAdsr()       { return adsr; }
    AdsrData& getFilterAdsr() { return filterAdsr; }
    
    float getFilterAdsrOutput() { return filterAdsrOutput; }
    
    void updateModParams (const int   filterType,
                          const float filterCutoff,
                          const float filterResonance,
                          const float adsrDepth,
                          const float lfoFreq,
                          const float lfoDepth);

private:
    static constexpr int numChannelsToProcess { 2 };
    std::array<OscData, numChannelsToProcess> osc1;
    std::array<OscData, numChannelsToProcess> osc2;
    std::array<FilterData, numChannelsToProcess> filter;
    std::array<dsp::Oscillator<float>, numChannelsToProcess> lfo;
    AdsrData adsr;
    AdsrData filterAdsr;
    AudioBuffer<float> synthBuffer;
    float filterAdsrOutput { 0.0f };
    std::array<float, numChannelsToProcess> lfoOutput { 0.0f, 0.0f };

    
    dsp::Gain<float> gain;
    bool isPrepared { false };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthVoice)
};
