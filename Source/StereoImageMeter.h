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
        auto bounds = getLocalBounds();
        goniometer.setBounds(bounds.removeFromTop(JUCE_LIVE_CONSTANT(220)));
        correlationMeter.setBounds(bounds);
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
