/*
  ==============================================================================

    MeterControlColumnR.cpp
    Created: 30 Sep 2022 1:49:11pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "MeterControlColumnR.h"

MeterControlColumnR::MeterControlColumnR() :
    scaleLabel ("scaleLabel", "SCALE"),
    histLabel ("histLabel", "HIST")
{
    scaleLabel.setFont(20.0);
    scaleLabel.setColour (juce::Label::textColourId, juce::Colours::orange);
    addAndMakeVisible(scaleLabel);
    
    scaleControl.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    scaleControl.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    scaleControl.setLookAndFeel(&scalerLookAndFeel);
    addAndMakeVisible(scaleControl);
    
    enableHoldButton.setColour(ComboBox::outlineColourId, juce::Colours::orange);
    addAndMakeVisible(enableHoldButton);
    
    addAndMakeVisible(holdControl);
    holdControl.setLookAndFeel(&scalerLookAndFeel);
    holdControl.addItem("0s", 1);
    holdControl.addItem("0.5s", 2);
    holdControl.addItem("2s", 3);
    holdControl.addItem("4s", 4);
    holdControl.addItem("6s", 5);
    holdControl.addItem("inf", 6);
    holdControl.setSelectedId(1);
    
    histLabel.setFont(20.0);
    histLabel.setColour (juce::Label::textColourId, juce::Colours::orange);
    addAndMakeVisible(histLabel);
    
    addAndMakeVisible(histControl);
    histControl.setLookAndFeel(&scalerLookAndFeel);
    histControl.addItem("Stacked", 1);
    histControl.addItem("Side-by-Side", 2);
    histControl.setSelectedId(1);
}


void MeterControlColumnR::paint (Graphics& g)
{
    //paint background
    g.setColour (Colours::black);
    g.setFont (15.0f);
    
    Rectangle<float> r(getLocalBounds().getX(), 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    g.fillRect(r);
    
    //Combo Box GUI Dividers RIGHT
    g.setColour (Colours::white);
    
    g.drawLine(getLocalBounds().getX(), scaleControl.getBottom() -10, getLocalBounds().getWidth(), scaleControl.getBottom() -10, 1);
    
    g.drawLine(getLocalBounds().getX(), holdControl.getBottom() + 10, getLocalBounds().getWidth(), holdControl.getBottom() + 10, 1);
    
    g.drawLine(getLocalBounds().getX(), histControl.getBottom() + 10, getLocalBounds().getWidth(), histControl.getBottom() + 10, 1);
}
void MeterControlColumnR::resized()
{
    scaleLabel.setBounds(getLocalBounds().getCentreX() - 40, 15, 70, 25);
    scaleControl.setBounds(getLocalBounds().getCentreX() - 50, scaleLabel.getBottom() - 30, 100, 100);
    
    enableHoldButton.setBounds(getLocalBounds().getCentreX() - 25, scaleControl.getBottom() + 10, 50, 25);
    holdControl.setBounds(getLocalBounds().getCentreX() - 40, enableHoldButton.getBottom(), 75, 25);
    
    histLabel.setBounds(getLocalBounds().getCentreX() - 25, holdControl.getBottom() + 10, 50, 25);
    histControl.setBounds(getLocalBounds().getCentreX() - 40, histLabel.getBottom(), 75, 25);
}
