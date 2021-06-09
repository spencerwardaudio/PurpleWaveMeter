/*
  ==============================================================================

    StereoMeter.cpp
    Created: 9 Jun 2021 2:35:27am
    Author:  Spencer Ward

  ==============================================================================
*/

#include "StereoMeter.h"

StereoMeter::StereoMeter()
{
    addAndMakeVisible(macroMeterLeft);
    
    addAndMakeVisible(dBScale);
    
    addAndMakeVisible(macroMeterRight);
    
    addAndMakeVisible(labelLR);
}

void StereoMeter::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::black);
    
    labelLR.setText("L Peak R", dontSendNotification);
}


void StereoMeter::resized()
{
    auto bounds = getLocalBounds();
    
    macroMeterLeft.setBounds(bounds);
    
    auto meterLeftEdge = macroMeterLeft.getRight();

    dBScale.setBounds(meterLeftEdge, bounds.getY(), bounds.getWidth(), bounds.getHeight());
    
    
//    dBScale.ticks = meterTicks.ticks;
//    dBScale.yOffset = meterTicks.getY();

    dBScale.setBounds(macroMeterLeft.getRight(), 0, 50, getHeight());
}
