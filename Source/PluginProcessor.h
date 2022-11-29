/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Fifo.h"

#define VerifyDbScale  true

class Pfmcpp_project10AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Pfmcpp_project10AudioProcessor();
    ~Pfmcpp_project10AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::Identifier parameter {"Params"};
    ValueTree valueTree {"parameter"};
    
    juce::Identifier decayRate {"decayRate"};
    juce::Identifier averageTime {"averageTime"};
    juce::Identifier meterView {"meterView"};
    juce::Identifier goniometerScale {"goniometerScale"};
    juce::Identifier holdTime {"holdTime"};
    juce::Identifier enableHold {"enableHold"};
    juce::Identifier histogramView {"histogramView"};
    
    juce::Identifier peakThreshold {"peakThreshold"};
    juce::Identifier rmsThreshold {"rmsThreshold"};

    Fifo<AudioBuffer<float>> fifo;
    int maxBufferSize {};
    
private:
    
#if VerifyDbScale
    dsp::Oscillator<float> oscl { [] (float x) { return std::sin (x); }};
#endif
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessor)
};
