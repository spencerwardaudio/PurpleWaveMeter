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
    
    decayRateControl.addItemList( {"-3dB/s", "-6dB/s", "-12dB/s", "-24dB/s", "-36dB/s" }, 1);
    decayRateControl.setSelectedId(1);
    decayRateControl.setLookAndFeel(&scalerLookAndFeel);
    addAndMakeVisible(decayRateControl);
    
    avgLabel.setFont(20.0);
    avgLabel.setColour (juce::Label::textColourId, TEXT_COLOUR);
    addAndMakeVisible(avgLabel);
    
    avgControl.addItemList( {"100ms", "250ms", "500ms", "1000ms", "2000ms" }, 1);
    avgControl.setSelectedId(1);
    avgControl.setLookAndFeel(&scalerLookAndFeel);
    addAndMakeVisible(avgControl);
    
    meterLabel.setFont(20.0);
    meterLabel.setColour (juce::Label::textColourId, TEXT_COLOUR);
    addAndMakeVisible(meterLabel);

    meterControl.addItemList( {"Both", "Peak", "Avg" }, 1);
    meterControl.setSelectedId(1);
    meterControl.setLookAndFeel(&scalerLookAndFeel);
    addAndMakeVisible(meterControl);
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
