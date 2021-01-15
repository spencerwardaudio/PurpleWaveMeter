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
//drawn with a solid color and orange hold tick
//Meter showing the Right AverageMeter showing the Right instant
//Label at the bottom showing the name of the meter

#define MaxDecibels  12.0
#define NegativeInfinity -66.0

struct ValueHolder : Timer
{
    ValueHolder()
    {
        startTimerHz(30);
    }
    
    ~ValueHolder()
    {
        stopTimer();
    }
    
    void timerCallback() override
    {
        auto currentTime = now.getCurrentTime();
        
        auto pkDiff = currentTime - peakTime;
        
        if (pkDiff.inMilliseconds() > holdTime)
            resetCurrentValue();
    }
    
    void setThreshold(float _threshold)
    {
        threshold = _threshold;
    }
    
    void updateHeldValue(float input)
    {
        if (input > threshold)
        {
            if(input > currentValue)
                currentValue = input;
                
            DBG( "peakValue Held: " << currentValue);
            peakTime = now.getCurrentTime();
        }
    }
    
    void setHoldTime(int ms)
    {
        holdTime = ms;
    }
    
    float getCurrentValue(float value)
    {
        if(value > threshold)
        {
            currentValue = value;
            DBG( "value > threshold: " << currentValue);
            
            updateHeldValue(value);
            return holdTime = value;
        }
        
        DBG( "value < threshold: " << value);
        return currentValue = threshold;
    }
    
private:
    
    void resetCurrentValue() { currentValue = threshold; }
    
    float currentValue {};

    float threshold {};
    
    int holdTime {};
    
    Time now;
    Time peakTime;
};


struct Tick
{
    int y = 0;
    int dB = 0;
};

struct DBScale : Component
{
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::black);
        g.setColour(Colours::white);
        
        Rectangle<int> r;
        r.setWidth(getWidth());
        r.setHeight(14);
        r.setX(0);
        r.setY(0);
    
        for(auto t : ticks)
        {
            g.drawSingleLineText(juce::String(t.dB), 0, t.y + yOffset);
        }
    }

    std::vector<Tick> ticks;
    int yOffset = 0;
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
        g.fillAll(Colours::white);
        
        auto bounds = getLocalBounds();
        
        auto h = bounds.getHeight();
        auto level = jmap((double)audioPassingVal, NegativeInfinity, MaxDecibels, 0.0, 1.0);
        
        g.setColour(Colours::blue);
        g.fillRect(bounds.withHeight(h * level).withY(h * (1.0 - level)));

        std::cout << level << std::endl;
        std::cout << h << std::endl;
    }
    
    void resized() override
    {
        auto h = getHeight();
        
        ticks.clear();
        Tick tck;
        
        for(int i = (int)NegativeInfinity; i <= (int)MaxDecibels; i += 6)
        {
            tck.y = jmap(i, (int)NegativeInfinity, (int)MaxDecibels, h, 0);
            std::cout << tck.y << " : y " << std::endl;
            tck.dB = i;
            std::cout << tck.dB << " : dB " << std::endl;
            
            ticks.push_back(tck);
        }
    }
    
    std::vector<Tick> ticks;
    
    float audioPassingVal {};
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
    
    ValueHolder valueHolder;
    Meter meter;
    DBScale dBScale;
    
    Pfmcpp_project10AudioProcessor& processor;
    AudioBuffer<float> editorBuffer;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessorEditor)
};
