/*
  ==============================================================================

    StereoMeter.h
    Created: 9 Jun 2021 2:35:27am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "MacroMeter.h"

#define DB_SCALE_WIDTH 25
#define MACRO_METER_WIDTH 40
#define STEREO_METER_WIDTH 105

struct StereoMeter : Component
{
    StereoMeter(const String MeterName);
    
    //provide setters for updating
    void update(int channel, float levelInDB);
    
    void paint (Graphics& g) override;
    void resized() override;
    
private:
    Rectangle<float> r;
    Path p;
    
    MacroMeter macroMeterLeft = MacroMeter(0, 30);
    MacroMeter macroMeterRight = MacroMeter(10, 0);
    DBScale dBScale;
    Label labelLR;
};
