/*
  ==============================================================================

    StereoMeter.h
    Created: 9 Jun 2021 2:35:27am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "MacroMeter.h"

#define DBScaleWidth 25
#define MacroMeterWidth 40
#define StereoMeterWidth 105


struct StereoMeter : Component
{
    StereoMeter(const String MeterName);
    
    void paint (Graphics& g) override;
    void resized() override;
    
    String name;
    
    Rectangle<float> r;
    Path p;
    
    MacroMeter macroMeterLeft = MacroMeter(0, 30);
    MacroMeter macroMeterRight = MacroMeter(10, 0);
    DBScale dBScale;
    Label labelLR;
};
