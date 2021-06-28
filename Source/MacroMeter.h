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

    std::vector<Tick> getDBTick();
    int getDBBounds();

private:

    int meterAVGPos { 30 };
    int meterIPos {0};
    
    TextMeter textMeter;
    Meter meterInstant;
    Meter meterAverage;
    
    Averager<float> averageValue { 10, NEGATIVE_INFINITY };
};
