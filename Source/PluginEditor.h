/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "StereoMeter.h"
#include "MeterControlColumnL.h"
#include "MeterControlColumnR.h"
#include "StereoImageMeter.h"
#include "HistogramContainer.h"
#include "ScalerLookAndFeel.h"

//==============================================================================
/**
*/

#define TimerHz 60
#define TimerMS 16.6

class PurpleWaveMeterAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    PurpleWaveMeterAudioProcessorEditor (PurpleWaveMeterAudioProcessor&);
    ~PurpleWaveMeterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void takeHoldVal();
    void setDecayValue();

private:

    StereoMeter stereoMeterRMS { "L  RMS  R" };
    StereoMeter stereoMeterPk { "L  Peak  R" };
    
    StereoImageMeter stereoImageMeter;
    
    HistogramContainer histogramContainer;

    PurpleWaveMeterAudioProcessor& processor;
    AudioBuffer<float> editorBuffer;
    
    MeterControlColumnL meterControlColumnL;
    MeterControlColumnR meterControlColumnR;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PurpleWaveMeterAudioProcessorEditor)
};
