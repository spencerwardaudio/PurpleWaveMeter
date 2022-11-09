/*
  ==============================================================================

    MeterControlColumnL.cpp
    Created: 30 Sep 2022 1:48:53pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "MeterControlColumnL.h"

MeterControlColumnL::MeterControlColumnL() : 
    decayLabel ("decayLabel", "DECAY"),
    avgLabel ("avgLabel", "AVG"),
    meterLabel ("meterLabel", "VIEW")
{
    decayLabel.setFont(20.0);
    decayLabel.setColour (juce::Label::textColourId, TEXT_COLOUR);
    addAndMakeVisible(decayLabel);
    
    addAndMakeVisible(decayRateControl);
    decayRateControl.setLookAndFeel(&scalerLookAndFeel);
    decayRateControl.addItem("-3dB/s", 1);
    decayRateControl.addItem("-6dB/s", 2);
    decayRateControl.addItem("-12dB/s", 3);
    decayRateControl.addItem("-24dB/s", 4);
    decayRateControl.addItem("-36dB/s", 5);
    decayRateControl.setSelectedId(1);
    
    avgLabel.setFont(20.0);
    avgLabel.setColour (juce::Label::textColourId, TEXT_COLOUR);
    addAndMakeVisible(avgLabel);
    
    addAndMakeVisible(avgControl);
    avgControl.setLookAndFeel(&scalerLookAndFeel);
    avgControl.addItem("100ms", 1);
    avgControl.addItem("250ms", 2);
    avgControl.addItem("500ms", 3);
    avgControl.addItem("1000ms", 4);
    avgControl.addItem("2000ms", 5);
    avgControl.setSelectedId(1);
    
    meterLabel.setFont(20.0);
    meterLabel.setColour (juce::Label::textColourId, TEXT_COLOUR);
    addAndMakeVisible(meterLabel);
    
    addAndMakeVisible(meterControl);
    meterControl.setLookAndFeel(&scalerLookAndFeel);
    meterControl.addItem("Both", 1);
    meterControl.addItem("Peak", 2);
    meterControl.addItem("Avg", 3);
    meterControl.setSelectedId(1);
}


void MeterControlColumnL::paint (Graphics& g)
{
    //paint background
    g.setColour (BACKGROUND_COLOUR);
    g.setFont (15.0f);
    
    Rectangle<float> r(getLocalBounds().getX(), 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    g.fillRect(r);
    
    //draw grey panel
    g.setColour (Colours::slategrey);
    Rectangle<float> greyBox(getLocalBounds().getX() + 5, 10, getLocalBounds().getWidth() - 10, getLocalBounds().getHeight() - 10);
    g.fillRoundedRectangle(greyBox, 6.0f);
    
    //Combo Box GUI Dividers LEFT
    g.setColour (Colours::white);
    
    auto xOffset = 10;
    
    g.drawLine(getLocalBounds().getX() + xOffset, decayRateControl.getBottom() + 10, getLocalBounds().getWidth() - xOffset, decayRateControl.getBottom() + 10, 1);
    
    g.drawLine(getLocalBounds().getX() + xOffset, avgControl.getBottom() + 10, getLocalBounds().getWidth() - xOffset, avgControl.getBottom() + 10, 1);
    
    g.drawLine(getLocalBounds().getX() + xOffset, meterControl.getBottom() + 10, getLocalBounds().getWidth() - xOffset, meterControl.getBottom() + 10, 1);
}
void MeterControlColumnL::resized()
{
    decayLabel.setBounds(getLocalBounds().getCentreX() -25, 15, 50, 25);
    decayRateControl.setBounds(getLocalBounds().getCentreX() - 40, decayLabel.getBottom(), 75, 25);
    
    avgLabel.setBounds(getLocalBounds().getCentreX() -25, decayRateControl.getBottom() + 20, 50, 25);
    avgControl.setBounds(getLocalBounds().getCentreX() - 40, avgLabel.getBottom(), 75, 25);
    
    meterLabel.setBounds(getLocalBounds().getCentreX() -25, avgControl.getBottom() + 20, 50, 25);
    meterControl.setBounds(getLocalBounds().getCentreX() - 40, meterLabel.getBottom(), 75, 25);
}
