/*
  ==============================================================================

    StereoImageMeter.h
    Created: 2 Oct 2022 7:14:00am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "CorrelationMeter.h"
#include "Goniometer.h"

struct StereoImageMeter : public Component
{
    StereoImageMeter(AudioBuffer<float>& editorBuffer, double sampleRate) : correlationMeter(editorBuffer, sampleRate), goniometer(editorBuffer)
    {
        addAndMakeVisible(goniometer);
        addAndMakeVisible(correlationMeter);
    }
    
    void paint();
    
    void resized()
    {
        const auto verticalOffset = 20;
        const auto h = 25;
        const auto gonioHeight = 235;
        auto bounds = getLocalBounds();
        
        goniometer.setBounds(bounds.removeFromTop(gonioHeight));
        correlationMeter.setBounds(0, goniometer.getHeight() - verticalOffset, getLocalBounds().getWidth(), h);
    }
    
    void update()
    {
        correlationMeter.update();
        goniometer.repaint();
    }
    
    private:
    
    CorrelationMeter correlationMeter;
    Goniometer goniometer;
};
