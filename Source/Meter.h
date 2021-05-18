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
#define REFRESH_RATE 60.f

struct ValueHolderBase : Timer
{
    ValueHolderBase() { startTimerHz(REFRESH_RATE); }
    
    ~ValueHolderBase() { stopTimer(); }
    
    void setHoldTime(int ms) { holdTime = ms; }
    
    float getCurrentValue() const { return currentValue; }
    
protected:
    
    float currentValue { (float)NegativeInfinity };
    
    int64 peakTime { 0 };
    int64 holdTime { 100 };
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
};


struct DecayingValueHolder : ValueHolderBase
{
    DecayingValueHolder()
    {
        setDecayRate(3);
    }
    
    void timerCallback() override
    {
        currentTime = Time::currentTimeMillis();
        elapsedTime = currentTime - peakTime;
        
        if(elapsedTime >= holdTime)
        {
            currentValue -= decayRate;
            DBG( "decayRate: " << decayRate );
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
        
    void setDecayRate(float decayRateSeconds)
    {
        decayRate = decayRateSeconds / REFRESH_RATE;
    }
    
private:
    
    int64 currentTime { 0 };
    int64 elapsedTime { 0 };
    double decayRate { 0 };
};


struct TextMeter : Component
{
    void update(float audioValue);

    void paint(Graphics& g) override;
    
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
    
    float level {};
    float previousVal {};
    
    DecayingValueHolder decayingValueHolder;
};
