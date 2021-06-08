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
    
    //MacroMeter
    TextMeter textMeter;
    Meter meterInstant;
    Meter meterAverage;
    DBScale dBScale;
    
    Averager<float> averageValue { 10, 0.f };
};
