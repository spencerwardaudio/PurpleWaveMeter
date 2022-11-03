/*
  ==============================================================================

    MeterControlColumnL.h
    Created: 30 Sep 2022 1:48:53pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScalerLookAndFeel.h"
#include "GlobalVisualDefines.h"

struct MeterControlColumnL : public Component
{
    MeterControlColumnL();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    ScalerLookAndFeel scalerLookAndFeel;
    
    Label    decayLabel;
    ComboBox decayRateControl;
    
    Label    avgLabel;
    ComboBox avgControl;
    
    Label    meterLabel;
    ComboBox meterControl;
};
