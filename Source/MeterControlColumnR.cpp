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
    scaleLabel.setColour (juce::Label::textColourId, TEXT_COLOUR);
    addAndMakeVisible(scaleLabel);
    
    scaleControl.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    scaleControl.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    scaleControl.setLookAndFeel(&scalerLookAndFeel);
    addAndMakeVisible(scaleControl);
    
    enableHoldButton.setColour(ComboBox::outlineColourId, TEXT_COLOUR);
    enableHoldButton.setColour(TextButton::buttonOnColourId, Colours::orange);
    addAndMakeVisible(enableHoldButton);
    
    holdControl.addItemList( {"0s", "0.5s", "2s", "4s", "6s", "inf" }, 1);
    holdControl.setSelectedId(1);
    holdControl.setLookAndFeel(&scalerLookAndFeel);
    addAndMakeVisible(holdControl);
    holdControl.setVisible(false);
    
    resetHoldButton.setLookAndFeel(&scalerLookAndFeel);
    resetHoldButton.setVisible(false);
    addAndMakeVisible(resetHoldButton);
    
    histLabel.setFont(20.0);
    histLabel.setColour (juce::Label::textColourId, TEXT_COLOUR);
    addAndMakeVisible(histLabel);

    histControl.addItemList( { "Stacked", "Side-by-Side" }, 1);
    histControl.setSelectedId(1);
    histControl.setLookAndFeel(&scalerLookAndFeel);
    addAndMakeVisible(histControl);
}

MeterControlColumnR::~MeterControlColumnR()
{
    scaleControl.setLookAndFeel(nullptr);
    holdControl.setLookAndFeel(nullptr);
    resetHoldButton.setLookAndFeel(nullptr);
    histControl.setLookAndFeel(nullptr);
    
    setLookAndFeel(nullptr);
}


void MeterControlColumnR::paint (Graphics& g)
{
    //paint background
    g.setColour (BACKGROUND_COLOUR);
    g.setFont (15.0f);
    
    Rectangle<float> r(getLocalBounds().getX(), 0, getLocalBounds().getWidth(), getLocalBounds().getHeight());
    g.fillRect(r);
    
    //draw grey panel
    g.setColour (PANEL_COLOUR);
    Rectangle<float> greyBox(getLocalBounds().getX() + 5, 10, getLocalBounds().getWidth() - 10, getLocalBounds().getHeight() - 10);
    g.fillRoundedRectangle(greyBox, 6.0f);
    
    //Combo Box GUI Dividers RIGHT
    g.setColour (Colours::white);
    
    auto xOffset = 10;
    
    g.drawLine(getLocalBounds().getX() + xOffset, scaleControl.getBottom() -10, getLocalBounds().getWidth() - xOffset, scaleControl.getBottom() -10, 1);
    
    g.drawLine(getLocalBounds().getX() + xOffset, resetHoldButton.getBottom() + 10, getLocalBounds().getWidth() - xOffset, resetHoldButton.getBottom() + 10, 1);
    
    g.drawLine(getLocalBounds().getX() + xOffset, histControl.getBottom() + 10, getLocalBounds().getWidth() - xOffset, histControl.getBottom() + 10, 1);
}

void MeterControlColumnR::resized()
{
    scaleLabel.setBounds(0, 10, getLocalBounds().getWidth(), 25);
    scaleLabel.setJustificationType(Justification::horizontallyCentred);
    
    scaleControl.setBounds(getLocalBounds().getCentreX() - 42, scaleLabel.getBottom() - 25, 85, 85);
    
    enableHoldButton.setBounds(getLocalBounds().getCentreX() - 25, scaleControl.getBottom() - 5, 50, 25);
    holdControl.setBounds(getLocalBounds().getCentreX() - 40, enableHoldButton.getBottom(), 75, 25);
    
    resetHoldButton.setBounds(getLocalBounds().getCentreX() - 40, holdControl.getBottom(), 75, 25);
    
    histLabel.setBounds(getLocalBounds().getCentreX() - 25, resetHoldButton.getBottom() + 10, 50, 25);
    histControl.setBounds(getLocalBounds().getCentreX() - 40, histLabel.getBottom(), 75, 25);
}
