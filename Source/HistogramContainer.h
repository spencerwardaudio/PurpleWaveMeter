/*
  ==============================================================================

    HistogramContainer.h
    Created: 30 Sep 2022 6:09:23pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "Histogram.h"

struct HistogramContainer : public Component
{
    HistogramContainer()
    {
        addAndMakeVisible(histogramRMS);
        addAndMakeVisible(histogramPeak);
    }
    
    void paint (Graphics& g) override
    {
        
    }
    
    void resized() override
    {
        //from the bottom of the stereo meter to the bottom of the application height / 2
        histogramRMS.setBounds(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight()/2);
        histogramPeak.setBounds(0, histogramRMS.getHeight(), getLocalBounds().getWidth(), histogramRMS.getHeight());
    }
    
    void setSideBySide()
    {
        FlexBox fb;
//                fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;

        fb.items.add (juce::FlexItem (histogramRMS).withMinHeight (getLocalBounds().getHeight()).withMaxWidth (getLocalBounds().getWidth()/2).withFlex (1));

        fb.items.add (juce::FlexItem (histogramPeak).withMinHeight (getLocalBounds().getHeight()).withMaxWidth (getLocalBounds().getWidth()/2).withFlex (1));

        fb.performLayout (getLocalBounds().toFloat());
    }
    
    void stack()
    {
        FlexBox fb;
//                fb.flexWrap = juce::FlexBox::Wrap::wrap;
        fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
        fb.alignContent = juce::FlexBox::AlignContent::center;

        fb.items.add (juce::FlexItem (histogramRMS).withMaxHeight (getLocalBounds().getHeight()/2).withMinWidth(getLocalBounds().getWidth()).withFlex (1));

        fb.items.add (juce::FlexItem (histogramPeak).withMaxHeight (getLocalBounds().getHeight()/2).withMinWidth (getLocalBounds().getWidth()).withFlex (1));

        fb.performLayout (getLocalBounds().toFloat());
    }

    Histogram histogramRMS { "RMS Histogram" };
    Histogram histogramPeak { "Peak Histogram" };
};
