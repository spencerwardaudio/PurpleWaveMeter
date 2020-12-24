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

struct Tick
{
    int y = 0;
    float dB = 0.f;
};

struct DBScale : Component
{
    void paint(Graphics& g) override;
    std::vector<Tick> ticks;
};

struct Meter : Component
{
    
    void update(float audioValue)
    {
        audioPassingVal = Decibels::gainToDecibels(audioValue);
        repaint();
    }
    
    void paint(Graphics& g) override
    {
        auto h = bounds.getHeight();
        auto level = jmap((double)audioPassingVal, NegativeInfinity, MaxDecibels, 0.0, 1.0);
        
        g.setColour(Colours::blue);
        g.fillRect(bounds.withHeight(h * level).withY(h * (1.0 - level)));

        std::cout << level << std::endl;
        std::cout << h << std::endl;
    }
    
    void resized() override
    {
        auto h = 300;
        
        Tick tck;
        for(int i = (int)NegativeInfinity; i < (int)MaxDecibels; i += 6)
        {
            tck.y = jmap(i, (int)NegativeInfinity, (int)MaxDecibels, h, 0);
            std::cout << tck.y << std::endl;
            
            ticks.push_back(tck);
        }
    }
    
    std::vector<Tick> ticks;
    
    float audioPassingVal {};
    const double NegativeInfinity = -66.0;
    const double MaxDecibels = 12.0;
    Rectangle<int> bounds {};
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
    DBScale dBscale;

    Pfmcpp_project10AudioProcessor& processor;
    AudioBuffer<float> editorBuffer;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessorEditor)
};
