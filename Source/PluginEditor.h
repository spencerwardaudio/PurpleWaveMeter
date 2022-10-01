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
#include "HistogramContainer.h"
#include "MeterControlColumnL.h"
#include "MeterControlColumnR.h"
#include "ScalerLookAndFeel.h"

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
    
    void stack()
    {
        FlexBox fb;
//                fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
        fb.alignContent = juce::FlexBox::AlignContent::flexStart;
        
        Rectangle<float> r(0.f, stereoMeterRMS.getBottom(), getLocalBounds().getWidth(), getLocalBounds().getHeight() - stereoMeterRMS.getBottom());

        fb.items.add (juce::FlexItem (histogramRMS).withMaxHeight (r.getHeight()/2).withMinWidth(getLocalBounds().getWidth()).withFlex (1));

        fb.items.add (juce::FlexItem (histogramPeak).withMaxHeight (r.getHeight()/2).withMinWidth (getLocalBounds().getWidth()).withFlex (1));


        fb.performLayout (r);
    }
    
    
    void setSideBySide()
    {
        FlexBox fb;
//                fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;

        fb.items.add (juce::FlexItem (histogramRMS).withMaxHeight(getLocalBounds().getHeight() - stereoMeterRMS.getBottom()).withMaxWidth(getLocalBounds().getWidth()/2).withFlex (1));

        fb.items.add (juce::FlexItem (histogramPeak).withMaxHeight(getLocalBounds().getHeight() - stereoMeterRMS.getBottom()).withMaxWidth(getLocalBounds().getWidth()/2).withFlex (1));

        Rectangle<float> r(0.f, stereoMeterRMS.getBottom(), getLocalBounds().getWidth(), getLocalBounds().getHeight() - stereoMeterRMS.getBottom());
        fb.performLayout (r);
    }

private:

    Histogram histogramRMS { "RMS Histogram" };
    Histogram histogramPeak { "Peak Histogram" };

    StereoMeter stereoMeterRMS { "L  RMS  R" };
    StereoMeter stereoMeterPk { "L  Peak  R" };
    
    StereoImageMeter stereoImageMeter;

    Pfmcpp_project10AudioProcessor& processor;
    AudioBuffer<float> editorBuffer;
    
    MeterControlColumnL meterControlColumnL;
    MeterControlColumnR meterControlColumnR;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessorEditor)
};
