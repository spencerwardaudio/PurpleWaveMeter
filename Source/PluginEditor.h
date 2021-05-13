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
        auto currentTime = Time::currentTimeMillis();
        
        auto pkDiff = currentTime - peakTime;
        
        if (pkDiff > holdTime)
            resetCurrentValue();
    }
    
    void setThreshold(float _threshold)
    {
        threshold = _threshold;
    }
    
    void updateHeldValue(float input)
    {
        if(input > threshold)
        {
            peakTime = Time::currentTimeMillis();

            if (currentValue < input)
            {
                currentValue = input;
            }
        }
    }
    
    void setHoldTime(int ms)
    {
        holdTime = ms;
    }
    
    float getCurrentValue() const { return currentValue; }
    
    bool isOverThreshold() const
    {
        return currentValue > threshold;
    }
    
private:
    
    float currentValue { (float)NegativeInfinity };
    
    void resetCurrentValue() { currentValue = threshold; }

    float threshold { 0 };
    
    int64 holdTime { 1 };
    
    int64 peakTime { 0 };
};


struct TextMeter : Component
{
    
    void update(float audioValue)
    {
        level = Decibels::gainToDecibels(audioValue);
        valueHolder.updateHeldValue(level);
        repaint();
    }

    void paint(Graphics& g) override
    {
        String str;
        
        auto over = valueHolder.isOverThreshold();
        if( over )
        {
            str = juce::String( valueHolder.getCurrentValue(), 1 );
        }
        else
        {
            str = ( level <= NegativeInfinity ) ? "-inf" : juce::String( level, 1 );
        }
        
        g.fillAll ( over ? Colours::red : Colours::black );
        g.setColour ( Colours::white );
        g.drawSingleLineText(str, 5, Justification::centred);
    }

    float level {};
    
    ValueHolder valueHolder;
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

        for(auto& tck : ticks )
        {
            g.setColour (juce::Colours::black);
            
            juce::Line<float> line (juce::Point<float> (2, tck.y - 3),
                                    juce::Point<float> (37, tck.y - 3));

            g.drawLine (line, 0.8f);
        }
        
        auto h = bounds.getHeight();
        auto level = jmap((double)audioPassingVal, NegativeInfinity, MaxDecibels, 0.0, 1.0);
        
        g.setColour(Colours::greenyellow);
        g.fillRect(bounds.withHeight(h * level).withY(h * (1.0 - level)));
    }
    
    void resized() override
    {
        auto h = getHeight();
        
        ticks.clear();
        Tick tck;
        
        for(int i = (int)NegativeInfinity; i <= (int)MaxDecibels; i += 6)
        {
            tck.y = jmap(i, (int)NegativeInfinity, (int)MaxDecibels, h, 0) + 4;
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
    
    TextMeter textMeter;
    Meter meter;
    DBScale dBScale;
    
    Pfmcpp_project10AudioProcessor& processor;
    AudioBuffer<float> editorBuffer;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessorEditor)
};
