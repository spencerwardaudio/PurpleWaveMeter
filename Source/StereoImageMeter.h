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
        goniometer.setBounds(getLocalBounds().removeFromTop(getHeight() - 10));
        correlationMeter.setBounds(getLocalBounds().removeFromBottom(20));
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
