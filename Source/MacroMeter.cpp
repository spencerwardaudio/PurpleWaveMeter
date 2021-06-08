/*
  ==============================================================================

    MacroMeter.cpp
    Created: 8 Jun 2021 9:44:17am
    Author:  Spencer Ward

  ==============================================================================
*/

#include "MacroMeter.h"


MacroMeter::MacroMeter()
{
    addAndMakeVisible(meterInstant);
    addAndMakeVisible(meterAverage);
    
    addAndMakeVisible(textMeter);
    addAndMakeVisible(dBScale);
    
    textMeter.valueHolder.setThreshold(0.f);
    textMeter.valueHolder.setHoldTime(300);
    
    meterInstant.decayingValueHolder.setHoldTime(1000);
    meterInstant.decayingValueHolder.setDecayRate(3.f);
    
    meterAverage.decayingValueHolder.setHoldTime(1000);
    meterAverage.decayingValueHolder.setDecayRate(3.f);
}

void MacroMeter::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::black);
}

void MacroMeter::resized()
{
    meterInstant.setBounds(0,
                    10,
                    30,
                    200);
    
    meterAverage.setBounds(30,
                           10,
                           10,
                           200);
    
    textMeter.setBounds(0,
                        -27,
                        40,
                        41);
    
    dBScale.ticks = meterInstant.ticks;
    dBScale.yOffset = meterInstant.getY();
    
    dBScale.setBounds(meterAverage.getRight(), 0, 50, getHeight());
}
