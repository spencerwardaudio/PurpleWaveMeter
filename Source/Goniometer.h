/*
  ==============================================================================

    Goniometer.h
    Created: 26 Aug 2022 4:39:56pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Fifo.h"

enum { defaultMinusInfinitydB = -100 };

struct Goniometer : Component
{
    Goniometer(AudioBuffer<float>& buffer);
    void paint(Graphics& g) override;
    void resized() override;
    
    Fifo<AudioBuffer<float>> fifoGonio;
    int maxBufferSize {};
    
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

template <typename Type>
static Type decibelsToGain (Type decibels,
                            Type minusInfinityDb = Type (defaultMinusInfinitydB))
{
    return decibels > minusInfinityDb ? std::pow (Type (10.0), decibels * Type (0.05))
                                      : Type();
}

template <typename Type>
static Type gainToDecibels (Type gain,
                            Type minusInfinityDb = Type (defaultMinusInfinitydB))
{
    return gain > Type() ? jmax (minusInfinityDb, static_cast<Type> (std::log10 (gain)) * Type (20.0))
                         : minusInfinityDb;
}
