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
    addAndMakeVisible(meter);
    addAndMakeVisible(dBScale);
    
    setSize (300, 400);
    
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

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void Pfmcpp_project10AudioProcessorEditor::resized()
{
    meter.setBounds(0,
                    JUCE_LIVE_CONSTANT(10),
                    50,
                    JUCE_LIVE_CONSTANT(200));
    
    meter.bounds.setSize(getWidth(), getHeight());
    
    dBScale.ticks = meter.ticks;
    dBScale.yOffset = meter.getY();
    
    dBScale.setBounds(meter.getRight(), 0, 50, getHeight());
    
}

void Pfmcpp_project10AudioProcessorEditor::timerCallback()
{
    if( processor.fifo.pull(editorBuffer) )
    {
        auto bufferLRMS = editorBuffer.getRMSLevel(0, 0, editorBuffer.getNumSamples());
        meter.update(bufferLRMS);
    }
}
