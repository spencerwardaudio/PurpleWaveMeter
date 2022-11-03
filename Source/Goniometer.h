/*
  ==============================================================================

    Goniometer.h
    Created: 26 Aug 2022 4:39:56pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum { defaultMinusInfinitydB = -100 };

struct Goniometer : Component
{
    Goniometer(AudioBuffer<float>& buffer);
    void paint(Graphics& g) override;
    void resized() override;
    
    void setGonioScaler(double val)
    {
        scaler = jmap(val, 0.0, 10.0, 0.5, 2.0);
    }
    
private:
    
    AudioBuffer<float>& _buffer;
    AudioBuffer<float> internalBuffer;
    Path p;
    int w, h;
    Point<int> center;
    
    double scaler { 0 };

    void drawBackground(Graphics& g);
    
    OpenGLContext openGLContext;
    
    juce::Image image;
};
