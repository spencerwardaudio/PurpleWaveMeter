/*
  ==============================================================================

    Meter.h
    Created: 14 May 2021 7:25:43am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define MaxDecibels  12.0
#define NegativeInfinity -66.0
#define TimerFrequency 30

struct ValueHolderBase : Timer
{
    ValueHolderBase() { startTimerHz(TimerFrequency); }
    
    ~ValueHolderBase() { stopTimer(); }
    
    float getCurrentValue() const { return currentValue; }
    
protected:
    
    float currentValue { (float)NegativeInfinity };
    
    int64 peakTime { 0 };
};



struct ValueHolder : ValueHolderBase
{
    
    void timerCallback() override
    {
        auto currentTime = Time::currentTimeMillis();
        
        auto pkDiff = currentTime - peakTime;
        
        if (pkDiff > holdTime)
            resetCurrentValue();
    }
    
    void setThreshold(float _threshold) { threshold = _threshold; }
    
    void setHoldTime(int ms) { holdTime = ms; }
    
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
    
    bool isOverThreshold() const { return currentValue > threshold; }
    
private:
    
    void resetCurrentValue() { currentValue = threshold; }

    float threshold { 0 };
    
    int64 holdTime { 1 };
    
};


struct DecayingValueHolder : ValueHolderBase
{
    
void timerCallback() override
{
    currentTime = Time::currentTimeMillis();
    elapsedTime = currentTime - peakTime;
    
    if(elapsedTime >= holdTime)
    {
        currentValue -= decayRate;
        DBG(currentValue << "dB");
    }
}

void updateHeldValue(float input)
{
    if(input > currentValue)
    {
        currentValue = input;
        peakTime = Time::currentTimeMillis();
    }
}
    
private:
    
    int64 currentTime = { 0 };
    int64 elapsedTime { 0 };
    
    //db per TimerFrequency interval
    int64 decayRate { 3 };
    
    int64 holdTime { TimerFrequency * 2 };
};


struct TextMeter : Component
{
    void update(float audioValue);

    void paint(Graphics& g) override;
    
    float level {};
    ValueHolder valueHolder;
    DecayingValueHolder decayingValueHolder;
};


struct Tick
{
    int y = 0;
    int dB = 0;
};

struct DBScale : Component
{
    void paint(Graphics& g) override;

    std::vector<Tick> ticks;
    int yOffset = 0;
};

struct Meter : Component
{
    void update(float audioValue);
    
    void paint(Graphics& g) override;
    
    void resized() override;
    
    std::vector<Tick> ticks;
    float audioPassingVal {};
};