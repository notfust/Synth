/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define SAPE SynthAudioProcessorEditor

using namespace juce;

SAPE::SynthAudioProcessorEditor (SynthAudioProcessor& p)
    : AudioProcessorEditor (&p)
    , audioProcessor (p)
    , osc1 (audioProcessor.apvts, "OSC1", "OSC1GAIN", "OSC1PITCH", "OSC1FMFREQ", "OSC1FMDEPTH")
    , osc2 (audioProcessor.apvts, "OSC2", "OSC2GAIN", "OSC2PITCH", "OSC2FMFREQ", "OSC2FMDEPTH")
    , filter (audioProcessor.apvts, "FILTERTYPE", "FILTERCUTOFF", "FILTERRESONANCE")
    , adsr (audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
    , lfo1 (audioProcessor.apvts, "LFO1FREQ", "LFO1DEPTH")
    , filterAdsr (audioProcessor.apvts, "FILTERATTACK", "FILTERDECAY", "FILTERSUSTAIN", "FILTERRELEASE")
{
    addAndMakeVisible (osc1);
    addAndMakeVisible (osc2);
    addAndMakeVisible (filter);
    addAndMakeVisible (adsr);
    addAndMakeVisible (lfo1);
    addAndMakeVisible (filterAdsr);
    
    startTimerHz (30);
    setSize (800, 400);
}

SAPE::~SynthAudioProcessorEditor() { stopTimer(); }

//==============================================================================
void SAPE::paint (Graphics& g)
{
    g.fillAll (Colours::lightcyan);
    float lineThickness = 2.0f;
    g.drawRect (0, 0, getWidth(), getHeight(), (int) lineThickness);

    //=========================================================================
    //Synth header
    {
        g.setFillType (Colours::lightgrey);
        g.fillRect (10, 10, 780, 50);

        g.setFillType (Colours::black);
        //Preset block
        g.drawRect (10.0f, 10.0f, 195.0f + lineThickness / 2.0f, 50.0f, lineThickness);
        g.drawRect (10.0f, 20.0f, 30.0f + lineThickness / 2.0f, 30.0f, lineThickness);
        g.drawRect (40.0f - lineThickness / 2.0f, 20.0f, 135 + lineThickness, 30.0f, lineThickness);
        g.drawRect (175.0f - lineThickness / 2.0f, 20.0f, 30 + lineThickness, 30.0f, lineThickness);

        //Synth name
        g.drawRect (205.0f - lineThickness / 2.0f, 10.0f, 240.0f + lineThickness, 50.0f, lineThickness);
        g.setFont (30.0f);
        g.drawText ("SYNTH", 205.0f - lineThickness / 2.0f, 10.0f, 240.0f + lineThickness, 50.0f, 36, true);

        //Wavetale
        g.drawRect (445.0f - lineThickness / 2.0f, 10.0f, 195.0f + lineThickness, 50.0f, lineThickness);
        g.drawRect (450.0f, 15.0f, 185.0f, 40.0f, lineThickness);

        //Master Level
        g.drawRect (640.0f - lineThickness / 2.0f, 10.0f, 150.0f + lineThickness, 50.0f, lineThickness);
    }
}

void SAPE::resized()
{
    const auto oscWidth  = 435;
    const auto oscHeight = 140;
    osc1.setBounds (10, 70, oscWidth, oscHeight);
    osc2.setBounds (10, osc1.getBottom() + 10, oscWidth, oscHeight);
    filter.setBounds (osc1.getRight(), 70, 345, oscHeight);
    lfo1.setBounds(filter.getRight() - 150, filter.getBottom() - 74, 150, 140);
    adsr.setBounds (osc2.getRight(), osc1.getBottom() + 10, 345, oscHeight);
}

void SAPE::timerCallback() { repaint(); }
