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
    : AudioProcessorEditor (&p), processor (p)
{
    editorBuffer.setSize(2, processor.maxBufferSize);
    
    editorBuffer.clear();
    
    circularBuffer.resize(1, 0);
    
    circularBuffer.clear(0);
    
    addAndMakeVisible(stereoMeterPk);
    addAndMakeVisible(stereoMeterRMS);
    
    setSize (400, 300);
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
        
        circularBuffer.write(levelDBRMSL);
        DBG(circularBuffer.getReadIndex());
    }
}
