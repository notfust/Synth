/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

#define SAP SynthAudioProcessor

using namespace juce;

SAP::SynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
     , apvts (*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound (new SynthSound());
    
    for (int i = 0; i < 5; i++)
    {
        synth.addVoice (new SynthVoice());
    }
}

SAP::~SynthAudioProcessor() {}

//==============================================================================
const String SAP::getName() const
{
    return JucePlugin_Name;
}

bool SAP::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SAP::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SAP::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SAP::getTailLengthSeconds() const { return 0.0; }

int SAP::getNumPrograms() { return 1; }

int SAP::getCurrentProgram() { return 0; }

void SAP::setCurrentProgram (int index) {}

const String SAP::getProgramName (int index) { return {}; }

void SAP::changeProgramName (int index, const String& newName) {}

//==============================================================================
void SAP::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void SAP::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SAP::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // Это место, где вы проверяете, поддерживается ли макет.
    // В этом коде шаблона мы поддерживаем только моно или стерео.
    // Некоторые хосты плагинов, такие как определенные версии GarageBand, будут
    // загружаем плагины, поддерживающие макеты стереофонических шин.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // Это проверяет, соответствует ли входной макет выходному макету
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SAP::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    setParams();
    
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    dsp::AudioBlock<float> block { buffer };
}

//==============================================================================
bool SAP::hasEditor() const { return true; }

AudioProcessorEditor* SAP::createEditor()
{
    return new SynthAudioProcessorEditor (*this);
}

//==============================================================================
void SAP::getStateInformation (MemoryBlock& destData) {}

void SAP::setStateInformation (const void* data, int sizeInBytes) {}

//==============================================================================
// Это создает новые экземпляры плагина..
AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new SynthAudioProcessor(); }

AudioProcessorValueTreeState::ParameterLayout SAP::createParams()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    
    // OSC select
    params.push_back (std::make_unique<AudioParameterChoice> ("OSC1", "Oscillator 1", StringArray {"Sine", "Saw", "Square", "Triangle"}, 0));
    params.push_back (std::make_unique<AudioParameterChoice> ("OSC2", "Oscillator 2", StringArray {"Sine", "Saw", "Square", "Triangle"}, 0));
    
    // OSC Gain
    params.push_back (std::make_unique<AudioParameterFloat>("OSC1GAIN", "Oscillator 1 Gain", NormalisableRange<float> { -40.0f, 0.2f, 0.1f }, 0.1f, "dB"));
    params.push_back (std::make_unique<AudioParameterFloat>("OSC2GAIN", "Oscillator 2 Gain", NormalisableRange<float> { -40.0f, 0.2f, 0.1f }, 0.1f, "dB"));
    
    // OSC Pitch val
    params.push_back (std::make_unique<AudioParameterInt>("OSC1PITCH", "Oscillator 1 Pitch", -48, 48, 0));
    params.push_back (std::make_unique<AudioParameterInt>("OSC2PITCH", "Oscillator 2 Pitch", -48, 48, 0));
    
    // FM Osc Freq
    params.push_back (std::make_unique<AudioParameterFloat>("OSC1FMFREQ", "Oscillator 1 FM Frequency", NormalisableRange<float> { 0.0f, 1000.0f, 0.1f }, 0.0f, "Hz"));
    params.push_back (std::make_unique<AudioParameterFloat>("OSC2FMFREQ", "Oscillator 1 FM Frequency", NormalisableRange<float> { 0.0f, 1000.0f, 0.1f }, 0.0f, "Hz"));
    
    // FM Osc Depth
    params.push_back (std::make_unique<AudioParameterFloat>("OSC1FMDEPTH", "Oscillator 1 FM Depth", NormalisableRange<float> { 0.0f, 100.0f, 0.1f }, 0.0f, ""));
    params.push_back (std::make_unique<AudioParameterFloat>("OSC2FMDEPTH", "Oscillator 2 FM Depth", NormalisableRange<float> { 0.0f, 100.0f, 0.1f }, 0.0f, ""));
    
    // LFO
    params.push_back (std::make_unique<juce::AudioParameterFloat>("LFO1FREQ", "LFO1 Frequency", juce::NormalisableRange<float> { 0.0f, 20.0f, 0.1f }, 0.0f, "Hz"));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("LFO1DEPTH", "LFO1 Depth", juce::NormalisableRange<float> { 0.0f, 10000.0f, 0.1f, 0.3f }, 0.0f, ""));
    
    //Filter
    params.push_back (std::make_unique<juce::AudioParameterChoice>("FILTERTYPE", "Filter Type", juce::StringArray { "Low Pass", "Band Pass", "High Pass" }, 0));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float> { 20.0f, 20000.0f, 0.1f, 0.6f }, 20000.0f, "Hz"));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("FILTERRESONANCE", "Filter Resonance", juce::NormalisableRange<float> { 0.1f, 2.0f, 0.1f }, 0.1f, ""));
    
    // ADSR
    params.push_back (std::make_unique<AudioParameterFloat>("ATTACK", "Attack", NormalisableRange<float> { 0.1f, 1.0f, 0.1f }, 0.1f));
    params.push_back (std::make_unique<AudioParameterFloat>("DECAY", "Decay", NormalisableRange<float> { 0.1f, 1.0f, 0.1f }, 0.1f));
    params.push_back (std::make_unique<AudioParameterFloat>("SUSTAIN", "Sustain", NormalisableRange<float> { 0.1f, 1.0f, 0.1f }, 1.0f));
    params.push_back (std::make_unique<AudioParameterFloat>("RELEASE", "Release", NormalisableRange<float> { 0.1f, 3.0f, 0.1f }, 0.4f));
    
    params.push_back (std::make_unique<AudioParameterFloat>("FILTERADSRDEPTH", "Filter ADSR Depth", NormalisableRange<float> { 0.0f, 10000.0f, 0.1f, 0.3f }, 10000.0f, ""));
    params.push_back (std::make_unique<AudioParameterFloat>("FILTERATTACK", "Filter Attack", NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.01f));
    params.push_back (std::make_unique<AudioParameterFloat>("FILTERDECAY", "Filter Decay", NormalisableRange<float> { 0.0f, 1.0f, 0.1f }, 0.1f));
    params.push_back (std::make_unique<AudioParameterFloat>("FILTERSUSTAIN", "Filter Sustain", NormalisableRange<float> { 0.0f, 1.0f, 0.1f }, 1.0f));
    params.push_back (std::make_unique<AudioParameterFloat>("FILTERRELEASE", "Filter Release", NormalisableRange<float> { 0.0f, 3.0f, 0.1f }, 0.1f));
    
    return { params.begin(), params.end() };
}

void SAP::setParams()
{
    setVoiceParams();
    setFilterParams();
}


void SAP::setVoiceParams()
{
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            auto& attack  = *apvts.getRawParameterValue ("ATTACK");
            auto& decay   = *apvts.getRawParameterValue ("DECAY");
            auto& sustain = *apvts.getRawParameterValue ("SUSTAIN");
            auto& release = *apvts.getRawParameterValue ("RELEASE");
            
            auto& osc1Choice  = *apvts.getRawParameterValue ("OSC1");
            auto& osc2Choice  = *apvts.getRawParameterValue ("OSC2");
            auto& osc1Gain    = *apvts.getRawParameterValue ("OSC1GAIN");
            auto& osc2Gain    = *apvts.getRawParameterValue ("OSC2GAIN");
            auto& osc1Pitch   = *apvts.getRawParameterValue ("OSC1PITCH");
            auto& osc2Pitch   = *apvts.getRawParameterValue ("OSC2PITCH");
            auto& osc1FmFreq  = *apvts.getRawParameterValue ("OSC1FMFREQ");
            auto& osc2FmFreq  = *apvts.getRawParameterValue ("OSC2FMFREQ");
            auto& osc1FmDepth = *apvts.getRawParameterValue ("OSC1FMDEPTH");
            auto& osc2FmDepth = *apvts.getRawParameterValue ("OSC2FMDEPTH");
            
            auto& filterAttack  = *apvts.getRawParameterValue ("FILTERATTACK");
            auto& filterDecay   = *apvts.getRawParameterValue ("FILTERDECAY");
            auto& filterSustain = *apvts.getRawParameterValue ("FILTERSUSTAIN");
            auto& filterRelease = *apvts.getRawParameterValue ("FILTERRELEASE");
            
            auto& osc1 = voice->getOscillator1();
            auto& osc2 = voice->getOscillator2();
            
            auto& adsr = voice->getAdsr();
            auto& filterAdsr = voice->getFilterAdsr();
            
            for (int i = 0; i < getTotalNumOutputChannels(); i++)
            {
                osc1[i].setParams (osc1Choice, osc1Gain, osc1Pitch, osc1FmFreq, osc1FmDepth);
                osc2[i].setParams (osc2Choice, osc2Gain, osc2Pitch, osc2FmFreq, osc2FmDepth);
            }
            
            adsr.updateParameters (attack.load(), decay.load(), sustain.load(), release.load());
            filterAdsr.updateParameters (filterAttack, filterDecay, filterSustain, filterRelease);
        }
    }
}

void SAP::setFilterParams()
{
    auto& filterType      = *apvts.getRawParameterValue ("FILTERTYPE");
    auto& filterCutoff    = *apvts.getRawParameterValue ("FILTERCUTOFF");
    auto& filterResonance = *apvts.getRawParameterValue ("FILTERRESONANCE");
    auto& adsrDepth       = *apvts.getRawParameterValue ("FILTERADSRDEPTH");
    auto& lfoFreq         = *apvts.getRawParameterValue ("LFO1FREQ");
    auto& lfoDepth        = *apvts.getRawParameterValue ("LFO1DEPTH");
        
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->updateModParams (filterType, filterCutoff, filterResonance, adsrDepth, lfoFreq, lfoDepth);
        }
    }
}
