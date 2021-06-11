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
    addAndMakeVisible(stereoMeterPk);
    addAndMakeVisible(stereoMeterRMS);
    
    setSize (600, 400);
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
    auto boundsRMS = getLocalBounds();
    auto boundsPk = getLocalBounds().removeFromRight(100);
    
    stereoMeterRMS.setBounds(boundsRMS);
    stereoMeterPk.setBounds(boundsPk);
}

void Pfmcpp_project10AudioProcessorEditor::timerCallback()
{
    if( processor.fifo.pull(editorBuffer) )
    {
        auto bufferLRPeak = editorBuffer.getMagnitude(0, 0, editorBuffer.getNumSamples());
        
        //MAKE DRY
        stereoMeterPk.macroMeterLeft.update(bufferLRPeak);
        stereoMeterPk.macroMeterRight.update(bufferLRPeak);
        
        stereoMeterRMS.macroMeterLeft.update(bufferLRPeak);
        stereoMeterRMS.macroMeterRight.update(bufferLRPeak);
    }
}
