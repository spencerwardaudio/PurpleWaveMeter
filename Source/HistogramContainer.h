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
        align(0);
    }
    
    void align(int formation)
    {
        const auto makeItem = [] (Component& comp)
        {
            return FlexItem { comp }.withFlex(0.5f).withMargin ({ 6 });
        };
        
        FlexBox box;
        
        if(formation == 0)
        {
            box.flexDirection = FlexBox::Direction::row;
        }
        else if (formation == 1)
        {
            box.flexDirection = FlexBox::Direction::column;
        }
        
        box.items = { makeItem (histogramRMS), makeItem (histogramPeak) };

        box.performLayout (getLocalBounds());
        
        histogramRMS.repaint();
    }
    
    private:
    
    Histogram histogramRMS { "RMS Histogram" };
    Histogram histogramPeak { "Peak Histogram" };

};
