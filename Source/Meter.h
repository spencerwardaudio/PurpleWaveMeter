/*
  ==============================================================================

    Meter.h
    Created: 14 May 2021 7:25:43am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define MAX_DECIBELS  12.0
#define NEGATIVE_INFINITY -66.0
#define REFRESH_RATE 60.f


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
        std::fill(elements.begin(), elements.end(), initialValue);
        
        avg = initialValue;
        
        runningSum = initialValue * elements.size();
    }
    
    //reset the vector, fill with the INITIAL VALUE, & call getAverage
    void resize(size_t s, T initialValue)
    {
        elements.resize(s);
        
        clear(initialValue);
    }

    //add an element to the vector & call getAverage
    void add(T t)
    {
        // cache the write index
        auto index = writeIndex.load();
        //update the running sum
        runningSum += t - elements[index];
        
        elements[index] = t;
        
        // cache the size locally, so getSize() isn't called more than once
        auto size = getSize();
        //update the average
        avg.store( runningSum / (float)size );
        
        //update the local write index
        ++index;
        //wrap the local write index if needed
        if( index > size - 1 )
            index = 0;
        
        // update the write index
        writeIndex.store(index);
    }
    

    float getAverage() const
    {
        return avg;
    }
    
    //query the size of the vector
    size_t getSize() const
    {
        return elements.size();
    }
    
private:
    
    std::vector<T> elements { };
    
    T runningSum { };
    std::atomic<size_t> writeIndex { 0 };
    std::atomic<float> avg { 0 };
};


struct ValueHolderBase : Timer
{
    ValueHolderBase() { startTimerHz(REFRESH_RATE); }
    
    ~ValueHolderBase() { stopTimer(); }
    
    void setHoldTime(int ms) { holdTime = ms; }
    
    float getCurrentValue() const { return currentValue; }
    
protected:
    
    float currentValue { (float)NEGATIVE_INFINITY };
    
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
    
    float level { NEGATIVE_INFINITY };
    
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
    
    float audioPassingVal { NEGATIVE_INFINITY };
    float level { NEGATIVE_INFINITY };
    
    DecayingValueHolder decayingValueHolder;
};
