/*
  ==============================================================================

    HistogramContainer.h
    Created: 18 Oct 2022 12:42:19pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "Histogram.h"

struct HistogramContainer : Component
{
    HistogramContainer()
    {
        addAndMakeVisible(histogramRMS);
        addAndMakeVisible(histogramPeak);
    }
    
    void setThreshold(float threshAsDecibels)
    {
        histogramRMS.setThreshold(threshAsDecibels);
        histogramPeak.setThreshold(threshAsDecibels);
    }
    
    void update(float RMSVal, float PkVal)
    {
        histogramRMS.update(RMSVal);
        histogramPeak.update(PkVal);
    }
    
    void paint();
    
    void resized()
    {
        stack();
    }
    
    void stack()
    {
        FlexBox fb;
            
        fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
        fb.alignContent = juce::FlexBox::AlignContent::flexStart;
        
        fb.items.add (juce::FlexItem (histogramRMS).withMaxHeight (getLocalBounds().getHeight()/2).withMinWidth(getLocalBounds().getWidth()).withFlex (1));

        fb.items.add (juce::FlexItem (histogramPeak).withMaxHeight(getLocalBounds().getHeight()/2).withMinWidth (getLocalBounds().getWidth()).withFlex (1));

        auto bounds = getLocalBounds();
        fb.performLayout (bounds);
    }
    
    
    void setSideBySide()
    {
        FlexBox fb;
        
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignContent = juce::FlexBox::AlignContent::center;

        fb.items.add (juce::FlexItem (histogramRMS).withMaxHeight(getLocalBounds().getHeight()).withMaxWidth(getLocalBounds().getWidth()/2).withFlex (1));

        fb.items.add (juce::FlexItem (histogramPeak).withMaxHeight(getLocalBounds().getHeight()).withMaxWidth(getLocalBounds().getWidth()/2).withFlex (1));

        auto bounds = getLocalBounds();
        fb.performLayout (bounds);
    }
    
    private:
    
    Histogram histogramRMS { "RMS Histogram" };
    Histogram histogramPeak { "Peak Histogram" };

};
