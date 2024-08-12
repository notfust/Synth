/*
  ==============================================================================

    AdsrData.h
    Created: 14 May 2023 3:46:11pm
    Author:  test

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

class AdsrData
    : public ADSR
{
public:
    void updateParameters (const float attack,
                           const float decay,
                           const float sustain,
                           const float release);
    
private:
    ADSR::Parameters adsrParams;
};
