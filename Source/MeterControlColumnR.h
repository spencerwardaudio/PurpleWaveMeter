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
#include "GlobalVisualDefines.h"

struct MeterControlColumnR : public Component
{
    MeterControlColumnR();
    ~MeterControlColumnR();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    ScalerLookAndFeel scalerLookAndFeel;
    
    Label    scaleLabel;
    Slider scaleControl;

    TextButton   enableHoldButton {"Hold"};
    bool        holdButtonOn { false };
    Colour buttonOff { TextButton::buttonColourId };
    
    ComboBox holdControl;
    TextButton   resetHoldButton {"Reset"};
    
    Label    histLabel;
    ComboBox histControl;

};
