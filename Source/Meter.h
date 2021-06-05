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

/*
 Averages a fixed number of elements
 allows you to change the number of elements
 */

template<typename T>
struct Averager
{
    Averager(size_t numElements, T initialValue)
    {
        resize(numElements, initialValue);
    }

    //reset elements in the vector to initial Value
    void clear(T initialValue)
    {
        std::fill(avgElements.begin(), avgElements.end(), initialValue);
        
        average = initialValue;
    }
    
    //reset the vector, fill with the INITIAL VALUE, & call getAverage
    void resize(size_t s, T initialValue)
    {
        avgElements.resize(s);
        
        clear(initialValue);
    }

    //add an element to the vector & call getAverage
    void add(T t)
    {
        runningSum += t - avgElements[writePointer];
        avgElements[writePointer] = t;
        
        auto local = this->average.load();
        
        local = getAverage();
        
        average = local;
        
        ++writePointer;
        
        if( writePointer > getSize())
            writePointer = 0;
    }
    
    //get average out of the vector
    float getAverage() const
    {
        auto tempAvg = runningSum / avgElements.size();
        
        return tempAvg;
    }
    
    //query the size of the vector
    size_t getSize() const
    {
        return avgElements.size();
    }
    
private:
    
    float runningSum { 0.f };
    
    std::atomic<float> average { 0 };
    std::atomic<int> writePointer { 0 };
    
    std::vector<float> avgElements { 0 };
};


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
