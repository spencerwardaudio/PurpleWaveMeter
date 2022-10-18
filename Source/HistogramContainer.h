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
        auto h = getLocalBounds().getHeight()/2;
        auto w = getLocalBounds().getWidth();
        
        const auto makeItem = [&h, &w] (Component& comp)
        {
            return FlexItem { comp }.withWidth (w).withHeight(h).withMargin ({ 6 });
        };

        FlexBox box;
        box.flexDirection = FlexBox::Direction::column;
        box.items = { makeItem (histogramRMS), makeItem (histogramPeak) };

        box.performLayout (getLocalBounds());
    }
    
    
    void setSideBySide()
    {
        auto h = getLocalBounds().getHeight();
        auto w = getLocalBounds().getWidth()/2;
        
        const auto makeItem = [&h, &w] (Component& comp)
        {
            return FlexItem { comp }.withWidth (w).withHeight(h).withMargin ({ 6 });
        };

        FlexBox box;
        box.flexDirection = FlexBox::Direction::row;
        box.items = { makeItem (histogramRMS), makeItem (histogramPeak) };

        box.performLayout (getLocalBounds());
    }
    
    private:
    
    Histogram histogramRMS { "RMS Histogram" };
    Histogram histogramPeak { "Peak Histogram" };

};
