/*
  ==============================================================================

    MeterControlColumnR.h
    Created: 30 Sep 2022 1:49:11pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ScalerLookAndFeel.h"

struct MeterControlColumnR : public Component
{
    MeterControlColumnR();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    ScalerLookAndFeel scalerLookAndFeel;
    
    Label    scaleLabel;
    Slider scaleControl;

    TextButton   enableHoldButton {"Hold"};
    ComboBox holdControl;
    
    Label    histLabel;
    ComboBox histControl;

};
