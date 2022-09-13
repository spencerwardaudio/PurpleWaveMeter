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
    
private:
    
    AudioBuffer<float>& _buffer;
    AudioBuffer<float> internalBuffer;
    Path p;
    int w, h;
    Point<int> center;

    void drawBackground(Graphics& g);
    
    juce::Image image;
    Graphics backgroundGraphic;
};
