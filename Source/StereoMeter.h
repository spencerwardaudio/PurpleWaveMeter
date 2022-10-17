/*
  ==============================================================================

    StereoMeter.h
    Created: 9 Jun 2021 2:35:27am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "MacroMeter.h"

#define DB_SCALE_WIDTH 25
#define MACRO_METER_WIDTH 40
#define STEREO_METER_WIDTH 105

class SliderLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SliderLookAndFeel()
    {
        setColour (juce::Slider::backgroundColourId, juce::Colours::transparentBlack);
        setColour (juce::Slider::trackColourId, juce::Colours::transparentBlack);
    }
    
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                          float sliderPos,
                          float minSliderPos,
                          float maxSliderPos,
                          const Slider::SliderStyle style, Slider& slider) override
    {
        //Drawing just the thumb
        Rectangle<float> rect(0.f, sliderPos, width, 2.f);
        g.setColour (Colours::red);
        g.fillRect(rect);
    }
};

struct StereoMeter : Component
{
    StereoMeter(const String MeterName);
    
    //provide setters for updating
    void update(int channel, float levelInDB);
    void setThreshold(float threshAsDecibels);
    
    void setDecayRate(float decay)
    {
        macroMeterLeft.setDecayRate(decay);
        macroMeterRight.setDecayRate(decay);
    }
    
    void setAverageDuration(float ms)
    {
        macroMeterLeft.setAverageDuration(ms);
        macroMeterRight.setAverageDuration(ms);
    }
    
    void paint (Graphics& g) override;
    void resized() override;
    
    juce::Slider thresholdSlider;
    
private:
    Rectangle<float> r;
    Path p;
    
    MacroMeter macroMeterLeft = MacroMeter(0, 30);
    MacroMeter macroMeterRight = MacroMeter(10, 0);
    DBScale dBScale;
    Label labelLR;
    
    SliderLookAndFeel sliderLookAndFeel;
};


