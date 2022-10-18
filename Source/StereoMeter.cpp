/*
  ==============================================================================

    StereoMeter.cpp
    Created: 9 Jun 2021 2:35:27am
    Author:  Spencer Ward

  ==============================================================================
*/

#include "StereoMeter.h"

StereoMeter::StereoMeter(const String MeterName) : labelLR( MeterName, MeterName )
{
    labelLR.setFont(20.0);
    
    labelLR.setColour (juce::Label::textColourId, juce::Colours::orange);
    
    addAndMakeVisible(macroMeterLeft);
    
    addAndMakeVisible(dBScale);
    
    addAndMakeVisible(macroMeterRight);
    
    addAndMakeVisible(labelLR);

    thresholdSlider.setSliderStyle (juce::Slider::SliderStyle::LinearVertical);
    
    thresholdSlider.setRange(NEGATIVE_INFINITY, MAX_DECIBELS);
    
    thresholdSlider.setValue(8.5f);
    
    thresholdSlider.setLookAndFeel(&sliderLookAndFeel);
    
    addAndMakeVisible(thresholdSlider);
}

void StereoMeter::update(int channel, float levelInDB)
{
    if(channel == 0)
        macroMeterLeft.update(levelInDB);
    
    else if(channel == 1)
        macroMeterRight.update(levelInDB);
}

void StereoMeter::paint (Graphics& g)
{
    g.setColour(Colours::black);
    g.fillRoundedRectangle(r, 10.f);
    
//    auto bounds = getLocalBounds();
//    g.setColour(Colours::red);
//    g.drawRect(bounds);
}


void StereoMeter::resized()
{
    auto labelBoxWidth = (2 * MACRO_METER_WIDTH) + DB_SCALE_WIDTH;

    r.setBounds(0,
                0,
                labelBoxWidth,
                METER_HEIGHT + 40);

    macroMeterLeft.setBounds(0,
                             0,
                             MACRO_METER_WIDTH,
                             METER_HEIGHT);

    dBScale.ticks = macroMeterLeft.getDBTick();
    dBScale.yOffset = macroMeterLeft.getDBBounds();

    dBScale.setBounds(macroMeterLeft.getRight(),
                      0,
                      DB_SCALE_WIDTH,
                      220);
                      //JUCE_LIVE_CONSTANT (250)
    
    thresholdSlider.setBounds(dBScale.getX(), macroMeterLeft.getY(), DB_SCALE_WIDTH, dBScale.getHeight());

    macroMeterRight.setBounds(MACRO_METER_WIDTH + DB_SCALE_WIDTH,
                              0,
                              MACRO_METER_WIDTH,
                              METER_HEIGHT);

    //reference size for positioning (not visible)
    Rectangle<int> labelRect;
    labelRect.setSize(STEREO_METER_WIDTH, 40);
    labelRect.setPosition(0, METER_HEIGHT + 5);

    labelLR.setJustificationType(Justification::centred);

    labelLR.setBounds(labelRect);
}

void StereoMeter::setThreshold(float threshAsDecibels)
{
    macroMeterLeft.setThreshold(threshAsDecibels);
    macroMeterRight.setThreshold(threshAsDecibels);
}


