/*
  ==============================================================================

    MacroMeter.h
    Created: 8 Jun 2021 9:41:03am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "Meter.h"

#define METER_HEIGHT 200
#define METER_WIDTH  30

struct MacroMeter : Component
{
    MacroMeter(int insMtrX, int avgMtrX);
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void update(float levelInDB);
    void setThreshold(float threshAsDecibels);
    
    void setHoldTimeINF(bool val)
    {
        meterInstant.setHoldTimeINF(val);
        meterAverage.setHoldTimeINF(val);
    }
    
    void setHoldTime(float time)
    {
        meterInstant.setHoldTime(time);
        meterAverage.setHoldTime(time);
    }
    
    void setDecayRate(float decay)
    {
        meterInstant.setDecayRate(decay);
        meterAverage.setDecayRate(decay);
    }
    
    void setAverageDuration(float valueDurationMS, float timerMS)
    {
        averageValue.resize(valueDurationMS / timerMS, NEGATIVE_INFINITY);
    }
    
    void setMeterVisibility(int meter)
    {
        //Both
        if(meter == 1)
        {
            meterInstant.setVisible(true);
            meterAverage.setVisible(true);
            
            meterInstant.setBounds(meterIPos,
                                   10,
                                   METER_WIDTH,
                                   METER_HEIGHT);
            
            meterAverage.setBounds(meterAVGPos,
                                   10,
                                   METER_WIDTH / 3,
                                   METER_HEIGHT);
        }
        //Peak
        else if(meter == 2)
        {
            meterInstant.setVisible(true);
            meterAverage.setVisible(false);
            
            meterInstant.setBounds(0, 10, METER_WIDTH + (METER_WIDTH / 3), METER_HEIGHT);
            meterInstant.repaint();
        }
        //Average
        else if(meter == 3)
        {
            meterInstant.setVisible(false);
            meterAverage.setVisible(true);
            
            meterAverage.setBounds(0, 10, METER_WIDTH + (METER_WIDTH / 3), METER_HEIGHT);
            meterAverage.repaint();
        }
    }
    
    void resetDecayingValueHolder()
    {
        meterInstant.resetDecayingValueHolder();
        meterAverage.resetDecayingValueHolder();
    }

    std::vector<Tick> getDBTick();
    int getDBBounds();

private:

    int meterAVGPos { 30 };
    int meterIPos { 0 };
    
    TextMeter textMeter;
    Meter meterInstant;
    Meter meterAverage;
    
    Averager<float> averageValue { 10, NEGATIVE_INFINITY };
};
