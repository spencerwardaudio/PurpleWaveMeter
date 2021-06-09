/*
  ==============================================================================

    MacroMeter.h
    Created: 8 Jun 2021 9:41:03am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "Meter.h"

struct MacroMeter : Component
{
    MacroMeter();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    void update(float levelInDB);
    
private:
    TextMeter textMeter;
    Meter meterInstant;
    Meter meterAverage;
    
    Averager<float> averageValue { 10, 0.f };
};
