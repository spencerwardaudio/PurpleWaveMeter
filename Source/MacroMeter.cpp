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
}

void MacroMeter::update(float levelInDB)
{
    meterInstant.update(levelInDB);
    textMeter.update(levelInDB);
    
    averageValue.add(levelInDB);
    
    auto avg = averageValue.getAverage();
    
    meterAverage.update(avg);
}
