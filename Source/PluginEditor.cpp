/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================


//==============================================================================
Pfmcpp_project10AudioProcessorEditor::Pfmcpp_project10AudioProcessorEditor (Pfmcpp_project10AudioProcessor& p)
: AudioProcessorEditor (&p), processor (p), stereoImageMeter(editorBuffer, p.getSampleRate())
{
    editorBuffer.setSize(2, processor.maxBufferSize);
    editorBuffer.clear();
    
    addAndMakeVisible(stereoMeterPk);
    addAndMakeVisible(stereoMeterRMS);
    
    addAndMakeVisible(stereoImageMeter);
    
    addAndMakeVisible(histogramContainer);
    
    addAndMakeVisible(meterControlColumnL);
    addAndMakeVisible(meterControlColumnR);
    
    meterControlColumnL.decayRateControl.onChange = [this]()
    {
//        StereoMeter RMS & Peak
        juce::String value = meterControlColumnL.decayRateControl.getText();
        float decay = value.dropLastCharacters(4).getFloatValue();
        DBG("selected" << value);
        
        stereoMeterRMS.setDecayRate(std::abs(decay));
        stereoMeterPk.setDecayRate(std::abs(decay));
    };
    
    meterControlColumnL.avgControl.onChange = [this]()
    {
        juce::String value = meterControlColumnL.avgControl.getText();
        float avgVal = value.dropLastCharacters(2).getFloatValue();
        DBG("selected" << avgVal);
        
        stereoMeterRMS.setAverageDuration(avgVal);
        stereoMeterPk.setAverageDuration(avgVal);
    };
    
    meterControlColumnR.histControl.onChange = [this]()
    {
        auto ID = meterControlColumnR.histControl.getSelectedId();
        
        if(ID == 1)
        {
            histogramContainer.align(0);
        }
        else if (ID == 2)
        {
            histogramContainer.align(1);
        }
    };
    
    stereoMeterRMS.thresholdSlider.onValueChange = [this]()
    {
       const auto newThreshold = stereoMeterRMS.thresholdSlider.getValue();

        //update the histogramRMS
        histogramContainer.setThreshold(newThreshold);
        
        //update the macrometersRMS
        stereoMeterRMS.setThreshold(newThreshold);
    };
    
    stereoMeterPk.thresholdSlider.onValueChange = [this]()
    {        
        const auto newThreshold = stereoMeterPk.thresholdSlider.getValue();

        //update the histogramPk
        histogramContainer.setThreshold(newThreshold);
        
        //update the macrometersPk
        stereoMeterPk.setThreshold(newThreshold);
    };
    
    setSize (650, 500);
    startTimerHz(TimerHz);
}

Pfmcpp_project10AudioProcessorEditor::~Pfmcpp_project10AudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void Pfmcpp_project10AudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setColour(Colours::lightskyblue);
    auto bounds = getLocalBounds();
    g.fillRect(bounds);
    
}

void Pfmcpp_project10AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    stereoMeterRMS.setBounds(bounds.removeFromLeft(100).removeFromTop(METER_HEIGHT + 40));
    stereoMeterPk.setBounds(bounds.removeFromRight(100).removeFromTop(METER_HEIGHT + 40));
    //from the bottom of the stereo meter to the bottom of the application height / 2
    
    meterControlColumnL.setBounds(bounds.removeFromLeft(stereoMeterRMS.getWidth()).removeFromTop(stereoMeterRMS.getHeight()));
    
    meterControlColumnR.setBounds(bounds.removeFromRight(stereoMeterRMS.getWidth()).removeFromTop(stereoMeterRMS.getHeight()));
    
    stereoImageMeter.setBounds(bounds.removeFromTop(meterControlColumnL.getHeight()));
    
    histogramContainer.setBounds(0, stereoMeterRMS.getHeight(), getLocalBounds().getWidth(), (getLocalBounds().getHeight() - stereoMeterRMS.getHeight()));
    
}

void Pfmcpp_project10AudioProcessorEditor::timerCallback()
{
    if( processor.fifo.pull(editorBuffer) )
    {
        auto levelGL = editorBuffer.getMagnitude(0, 0, editorBuffer.getNumSamples());
        auto levelGR = editorBuffer.getMagnitude(1, 0, editorBuffer.getNumSamples());
        
        //convert from gain to decibels
        auto levelDBL = Decibels::gainToDecibels(levelGL);
        auto levelDBR = Decibels::gainToDecibels(levelGR);
        
        auto levelGRMSL = editorBuffer.getRMSLevel(0, 0, editorBuffer.getNumSamples());
        auto levelGRMSR = editorBuffer.getRMSLevel(1, 0, editorBuffer.getNumSamples());
        
        auto levelDBRMSL = Decibels::gainToDecibels(levelGRMSL);
        auto levelDBRMSR = Decibels::gainToDecibels(levelGRMSR);
        
        stereoMeterPk.update(0, levelDBL);
        stereoMeterPk.update(1, levelDBR);
        
        stereoMeterRMS.update(0, levelDBRMSL);
        stereoMeterRMS.update(1, levelDBRMSR);
        
        auto avgRMS = (levelDBRMSL + levelDBRMSR) / 2;
        auto avgPeak = (levelDBL + levelDBR) / 2;

        histogramContainer.update(avgRMS, avgPeak);
        
        stereoImageMeter.update();
    }
    else
    {
        //Fade
        editorBuffer.applyGain( Decibels::decibelsToGain(-3.f) );
    }
    
    stereoImageMeter.repaint();
}
