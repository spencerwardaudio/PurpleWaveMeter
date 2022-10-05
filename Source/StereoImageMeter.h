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
        goniometer.setBounds(0, 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
        
        correlationMeter.setBounds(goniometer.getX() + 10, goniometer.getHeight() - 20, getLocalBounds().getWidth() - 20, 20);
    }
    void update()
    {
        correlationMeter.update();
    }
    
    private:
    
    CorrelationMeter correlationMeter;
    Goniometer goniometer;
};
