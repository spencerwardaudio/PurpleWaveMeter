/*
  ==============================================================================

    Histogram.h
    Created: 21 Aug 2022 9:08:31pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ReadAllAfterWriteCircularBuffer.h"
#include "Meter.h"

#define HISTOGRAM_HEIGHT 100
#define MAX_DB  12.0f
#define NEG_INF -66.0f

struct Histogram : Component
{
    Histogram(const String& title_);
    
    void paint(Graphics& g) override;
    void resized() override;
    void mouseDown(const MouseEvent& e) override;
    void update(float value);
    void setThreshold(float threshAsDecibels);
    
private:

    ReadAllAfterWriteCircularBuffer<float> buffer {float(NEG_INF)};
    Path path;
    
    void displayPath(Graphics& g, Rectangle<float> bounds);
    
    static Path buildPath(Path& p,
                          ReadAllAfterWriteCircularBuffer<float>& buffer,
                          Rectangle<float> bounds);
    
    void drawColourGradientRect(Graphics& g, Rectangle<int> bounds);
    void drawBackground(Graphics& g);
    
    juce::Image image;
    juce::Image imagebackground;
    
    const String title;
    
    DBScale dBScale;
    
    bool dBLevelClip = false;
    
    float threshold;
    
    std::vector<float> levels;
    size_t count{};
};



