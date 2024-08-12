/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 8 May 2023 9:49:20am
    Author:  test

  ==============================================================================
*/

#include "AdsrUI.h"

using namespace juce;

using SliderStyle = Slider::SliderStyle;

AdsrComponent::AdsrComponent (APVTS& apvts,
                              String attackId,
                              String decayId,
                              String sustainId,
                              String releaseId)
    : attack  ("Attack",  attackId,  apvts, dialWidth, dialHeight, SliderStyle::RotaryHorizontalVerticalDrag)
    , decay   ("Decay",   decayId,   apvts, dialWidth, dialHeight, SliderStyle::RotaryHorizontalVerticalDrag)
    , sustain ("Sustain", sustainId, apvts, dialWidth, dialHeight, SliderStyle::RotaryHorizontalVerticalDrag)
    , release ("Release", releaseId, apvts, dialWidth, dialHeight, SliderStyle::RotaryHorizontalVerticalDrag)
{
    addAndMakeVisible (attack);
    addAndMakeVisible (decay);
    addAndMakeVisible (sustain);
    addAndMakeVisible (release);
}

AdsrComponent::~AdsrComponent() {}

void AdsrComponent::paint (Graphics& g)
{
    float lineThickness = 2.0f;
    g.setFillType (Colours::lightgrey);
    g.fillRect (0, 0, 345, 140);
    
    //OSC 1 Filter
    {
        g.setFillType (Colours::black);
        g.drawRect (0.0f - lineThickness / 2.0f, 0.0f, 195.0f + lineThickness, 140.0f, lineThickness);

//        g.drawRect (0.0f, 5.0f, 20.0f + lineThickness / 2.0f, 20.0f, lineThickness);
//        g.drawRect (20.0f - lineThickness / 2.0f, 5.0f, 20.0f + lineThickness, 20.0f, lineThickness);
//        g.drawRect (40.0f - lineThickness / 2.0f, 5.0f, 135.0f + lineThickness, 20.0f, lineThickness);
//        g.drawRect (175.0f - lineThickness / 2.0f, 5.0f, 20.0f + lineThickness, 20.0f, lineThickness);

//        g.setFillType (Colours::grey);
//        g.fillRect (20.0f, 30.0f, 175.0f, 10.0f);
//        g.fillRect (20.0f, 125.0f, 175.0f, 10.0f);

        g.setFillType (Colours::black);
//        g.drawRect (0.0f, 30.0f, 20.0f + lineThickness / 2.0f, 105.0f, lineThickness);
//        g.drawRect (20.0f - lineThickness / 2.0f, 30.0f, 175.0f + lineThickness, 105.0f, lineThickness);

        g.saveState();
        AffineTransform af;
        g.addTransform (af.rotation(-MathConstants<float>::halfPi, getWidth() * 0.5f, getHeight() * 0.5f));
        g.setFont (20.0f);
        g.drawText ("ADSR", getWidth() * 0.5f - 50.0f, getHeight() * 0.5f - 172.5f, 105, 20, Justification::centred, true);
        
        g.restoreState();
    }
    
    //OSC 1 Filter param
    g.drawRect (195.0f - lineThickness / 2.0f, 0.0f, 150.0f + lineThickness, 140.0f, lineThickness);
}


void AdsrComponent::resized()
{
    const auto startX = 15;
    const auto startY = 0;
    const auto width  = 75;
    const auto height = 70;
    
    attack .setBounds (startX, startY, width, height);
    decay  .setBounds (attack.getRight(), startY, width, height);
    sustain.setBounds (startX, attack.getHeight(), width, height);
    release.setBounds (sustain.getRight(), attack.getHeight(), width, height);
}

