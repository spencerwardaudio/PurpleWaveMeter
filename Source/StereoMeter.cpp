/*
  ==============================================================================

    StereoMeter.cpp
    Created: 9 Jun 2021 2:35:27am
    Author:  Spencer Ward

  ==============================================================================
*/

#include "StereoMeter.h"

StereoMeter::StereoMeter(const String MeterName)
{
    name = MeterName;
    
    labelLR.setText(MeterName, dontSendNotification);
    labelLR.setFont(20.0);
    
    labelLR.setColour (juce::Label::textColourId, juce::Colours::orange);
    
    addAndMakeVisible(macroMeterLeft);
    
    addAndMakeVisible(dBScale);
    
    addAndMakeVisible(macroMeterRight);
    
    addAndMakeVisible(labelLR);
}

void StereoMeter::paint (Graphics& g)
{
    g.setColour(Colours::black);
    g.fillRoundedRectangle(r, 10.f);
}


void StereoMeter::resized()
{
    auto bounds = getLocalBounds();
    
    auto labelBoxWidth = (2 * MacroMeterWidth) + DBScaleWidth;
    
    r.setBounds(0, 0, labelBoxWidth, meterHeight + 40);

    macroMeterLeft.setBounds(bounds.getX(),
                             0,
                             MacroMeterWidth,
                             meterHeight);

    dBScale.ticks = macroMeterLeft.getDBTick();
    dBScale.yOffset = macroMeterLeft.getDBBounds();

    dBScale.setBounds(macroMeterLeft.getRight(),
                      0,
                      DBScaleWidth,
                      220);
                      //JUCE_LIVE_CONSTANT (250)

    macroMeterRight.setBounds(MacroMeterWidth + DBScaleWidth,
                              0,
                              MacroMeterWidth,
                              meterHeight);
    
    //reference size for positioning (not visible)
    Rectangle<int> labelRect;
    labelRect.setSize(StereoMeterWidth, 40);
    labelRect.setPosition(0, meterHeight + 5);
    
    labelLR.setJustificationType(Justification::centred);

    labelLR.setBounds(labelRect);
}
