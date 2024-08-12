/*
  ==============================================================================

    AdsrData.cpp
    Created: 14 May 2023 3:46:11pm
    Author:  test

  ==============================================================================
*/

#include "AdsrData.h"

void AdsrData::updateParameters (const float attack,
                                 const float decay,
                                 const float sustain,
                                 const float release)
{
    adsrParams.attack  = attack;
    adsrParams.decay   = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    setParameters (adsrParams);
}
