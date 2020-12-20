/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/

//TODO
//TextMeter at the top
//DbScale in the middle
//Meter showing the Left Instant,
//with ticks drawn behind the translucent gradient
//Meter showing the Left average,
//drawn with a solid color and orang hold tick
//Meter showing the Right AverageMeter showing the Right instant
//Label at the bottom showing the name of the meter

struct Meter : public Component
{
    Meter(int x, int y, int width, int height) : bounds(x, y, width, height)
    {
        
    }
    ~Meter();
    
    void update(float audioValue)
    {
        audioPassingVal = Decibels::gainToDecibels(audioValue);
        repaint();
    }
    
    void paint(Graphics& g) override
    {
        auto h = bounds.getHeight();
        auto level = (double)audioPassingVal;
        
        g.setColour(Colours::lightblue);
        g.fillRect(bounds.withHeight(h * level).withY(h * (1.0 - level)));
        std::cout << level << std::endl;
        std::cout << h << std::endl;
        
    }
    
    void resized() override
    {

    }
    
    
    float audioPassingVal {};
//    static constexpr double NegativeInfinity = -38.0;
//    static const double MaxDecibels = 0.0;
    Rectangle<int> bounds (x, y, width, height);
};



class Pfmcpp_project10AudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    Pfmcpp_project10AudioProcessorEditor (Pfmcpp_project10AudioProcessor&);
    ~Pfmcpp_project10AudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:
    
    Meter meter;

    Pfmcpp_project10AudioProcessor& processor;
    AudioBuffer<float> editorBuffer;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessorEditor)
};
