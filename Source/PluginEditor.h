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


//==============================================================================
/**
*/

class Pfmcpp_project10AudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    Pfmcpp_project10AudioProcessorEditor (Pfmcpp_project10AudioProcessor&);
    ~Pfmcpp_project10AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    
    const String strPk = "L  Peak  R";
    const String strRMS = "L  RMS  R";
    
    StereoMeter stereoMeterPk = StereoMeter(strPk);
    StereoMeter stereoMeterRMS = StereoMeter(strRMS);
    
    Pfmcpp_project10AudioProcessor& processor;
    AudioBuffer<float> editorBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessorEditor)
};
