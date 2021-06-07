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
    addAndMakeVisible(meterInstant);
    addAndMakeVisible(meterAverage);
    
    addAndMakeVisible(textMeter);
    addAndMakeVisible(dBScale);
    
    setSize (300, 400);
    
    startTimerHz(30);
    
    textMeter.valueHolder.setThreshold(0.f);
    textMeter.valueHolder.setHoldTime(300);
    
    meterInstant.decayingValueHolder.setHoldTime(1000);
    meterInstant.decayingValueHolder.setDecayRate(3.f);
    
    meterAverage.decayingValueHolder.setHoldTime(1000);
    meterAverage.decayingValueHolder.setDecayRate(3.f);
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
    meterInstant.setBounds(0,
                    10,
                    30,
                    200);
    
    meterAverage.setBounds(30,
                           10,
                           10,
                           200);
    
    textMeter.setBounds(0,
                        -27,
                        40,
                        41);
    
    dBScale.ticks = meterInstant.ticks;
    dBScale.yOffset = meterInstant.getY();
    
    dBScale.setBounds(meterAverage.getRight(), 0, 50, getHeight());
}

void Pfmcpp_project10AudioProcessorEditor::timerCallback()
{
    if( processor.fifo.pull(editorBuffer) )
    {
        auto bufferLRPeak = editorBuffer.getMagnitude(0, 0, editorBuffer.getNumSamples());
        
        meterInstant.update(bufferLRPeak);
        textMeter.update(bufferLRPeak);
        
        averageValue.add(bufferLRPeak);
        
        auto avg = averageValue.getAverage();
        
        meterAverage.update(avg);
    }
}
