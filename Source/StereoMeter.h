/*
  ==============================================================================

    StereoMeter.h
    Created: 9 Jun 2021 2:35:27am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "MacroMeter.h"

struct StereoMeter : Component
{
    StereoMeter();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    MacroMeter macroMeterLeft, macroMeterRight;
    DBScale dBScale;
    Label labelLR;
};
