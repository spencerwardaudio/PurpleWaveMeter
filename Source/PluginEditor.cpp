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
    addAndMakeVisible(textMeter);
    addAndMakeVisible(dBScale);
    
    setSize (300, 400);
    
    startTimerHz(30);
    
    textMeter.valueHolder.setThreshold(0.f);
    textMeter.valueHolder.setHoldTime(300);
    
    meter.decayingValueHolder.setHoldTime(1000);
    meter.decayingValueHolder.setDecayRate(3.f);
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
                    10,
                    40,
                    200);
    
    textMeter.setBounds(0,
                        -27,
                        40,
                        41);
    
    dBScale.ticks = meter.ticks;
    dBScale.yOffset = meter.getY();
    
    dBScale.setBounds(meter.getRight(), 0, 50, getHeight());
}

void Pfmcpp_project10AudioProcessorEditor::timerCallback()
{
    if( processor.fifo.pull(editorBuffer) )
    {
        auto bufferLRPeak = editorBuffer.getMagnitude(0, 0, editorBuffer.getNumSamples());
        
        meter.update(bufferLRPeak);
        textMeter.update(bufferLRPeak);
    }
}
