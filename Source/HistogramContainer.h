/*
  ==============================================================================

    HistogramContainer.h
    Created: 18 Oct 2022 12:42:19pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "Histogram.h"

enum class Formation
{
    row,
    column
};

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
        
    }
    
    void align(Formation orientation)
    {
        const auto makeItem = [] (Component& comp)
        {
            return FlexItem { comp }.withFlex(0.5f).withMargin ({ 6 });
        };
        
        FlexBox box;
        
        if(orientation == Formation::row)
        {
            box.flexDirection = FlexBox::Direction::row;
        }
        else if (orientation == Formation::column)
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
