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
: AudioProcessorEditor (&p), stereoImageMeter(editorBuffer, p.getSampleRate()), processor (p)
{
    editorBuffer.setSize(2, processor.maxBufferSize);
    editorBuffer.clear();
    
    addAndMakeVisible(stereoMeterPk);
    addAndMakeVisible(stereoMeterRMS);
    
    addAndMakeVisible(histogramRMS);
    addAndMakeVisible(histogramPeak);
    
    addAndMakeVisible(stereoImageMeter);
    
    stereoMeterRMS.thresholdSlider.onValueChange = [this]()
    {
       const auto newThreshold = stereoMeterRMS.thresholdSlider.getValue();

        //update the histogramRMS
        histogramRMS.setThreshold(newThreshold);
        
        //update the macrometersRMS
        stereoMeterRMS.setThreshold(newThreshold);
    };
    
    stereoMeterPk.thresholdSlider.onValueChange = [this]()
    {        
        const auto newThreshold = stereoMeterPk.thresholdSlider.getValue();

        //update the histogramPk
        histogramPeak.setThreshold(newThreshold);
        
        //update the macrometersPk
        stereoMeterPk.setThreshold(newThreshold);
    };
    
    setSize (450, 450);
    startTimerHz(30);
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

    g.setColour (Colours::black);
    g.setFont (15.0f);
}

void Pfmcpp_project10AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    stereoMeterRMS.setBounds(bounds.removeFromLeft(100).removeFromTop(METER_HEIGHT + 40));
    stereoMeterPk.setBounds(bounds.removeFromRight(100).removeFromTop(METER_HEIGHT + 40));
    
    //from the bottom of the stereo meter to the bottom of the application height / 2
    histogramRMS.setBounds(0, stereoMeterRMS.getBottom(), getWidth(), (bounds.getHeight() - stereoMeterPk.getBottom()) / 2);
    histogramPeak.setBounds(0, histogramRMS.getBottom(), getWidth(), (bounds.getHeight() - stereoMeterPk.getBottom()) / 2);
    
    stereoImageMeter.setBounds(stereoMeterRMS.getWidth(), 0, bounds.getWidth(), stereoMeterRMS.getHeight());
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

        histogramRMS.update(avgRMS);
        histogramPeak.update(avgPeak);
        
        stereoImageMeter.update();
    }
    else
    {
        //Fade
        editorBuffer.applyGain( Decibels::decibelsToGain(-3.f) );
    }
    
    stereoImageMeter.repaint();
}
