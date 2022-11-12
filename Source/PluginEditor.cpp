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
: AudioProcessorEditor (&p), stereoImageMeter(editorBuffer, p.getSampleRate()), processor(p)
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
//        StereoMeter RMS & Peak Decay
        setDecayValue();
    };
    
    meterControlColumnL.avgControl.onChange = [this]()
    {
        juce::String value = meterControlColumnL.avgControl.getText();
        float avgVal = value.dropLastCharacters(2).getFloatValue();
        
        stereoMeterRMS.setAverageDuration(avgVal, TimerMS);
        stereoMeterPk.setAverageDuration(avgVal, TimerMS);
    };
    
    meterControlColumnL.meterControl.onChange = [this]()
    {
        auto ID = meterControlColumnL.meterControl.getSelectedId();
    
        stereoMeterRMS.setMeterVisibility(ID);
        stereoMeterPk.setMeterVisibility(ID);
    };
    
    meterControlColumnR.scaleControl.onValueChange = [this]()
    {
        //get the slider change
        auto val = meterControlColumnR.scaleControl.getValue();
        //set this slider value as a multiplyer in the goniometer
        stereoImageMeter.setGonioScaler(val);
    };
    
    meterControlColumnR.histControl.onChange = [this]()
    {
        auto ID = meterControlColumnR.histControl.getSelectedId();
        
        if(ID == 1)
        {
            histogramContainer.align(Formation::column);
        }
        else if (ID == 2)
        {
            histogramContainer.align(Formation::row);
        }
    };
    
    meterControlColumnR.enableHoldButton.onStateChange = [this]()
    {
        if(meterControlColumnR.enableHoldButton.getState() == Button::ButtonState::buttonDown)
        {
            stereoMeterPk.displayTick();
            stereoMeterRMS.displayTick();
            
            if(!meterControlColumnR.holdButtonOn)
            {
                meterControlColumnR.holdControl.setVisible(true);
                meterControlColumnR.enableHoldButton.setColour (TextButton::buttonColourId, Colours::orange);
                meterControlColumnR.holdButtonOn = true;
                
                takeHoldVal();
            }
            else
            {
                meterControlColumnR.holdControl.setVisible(false);
                meterControlColumnR.enableHoldButton.removeColour(TextButton::buttonColourId);
                meterControlColumnR.resetHoldButton.setVisible(false);
                meterControlColumnR.holdButtonOn = false;
                
                //set hold time to zero
                stereoMeterRMS.setHoldTime(0);
                stereoMeterPk.setHoldTime(0);
                
                stereoMeterRMS.setHoldTimeINF(false);
                stereoMeterPk.setHoldTimeINF(false);
            }
        }
    };
    
    meterControlColumnR.resetHoldButton.onStateChange = [this]()
    {
        if(meterControlColumnR.resetHoldButton.getState() == Button::ButtonState::buttonDown)
        {
            stereoMeterRMS.resetDecayingValueHolder();
            stereoMeterPk.resetDecayingValueHolder();
        }
    };
    
    meterControlColumnR.holdControl.onChange = [this]()
    {
        takeHoldVal();
        
        setDecayValue();
    };
    
    //TODO reset the DecayingValueHolder's internal values to NEGATIVE_INFINITY
    
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
    
    stereoMeterRMS.setBounds(bounds.removeFromLeft(105).removeFromTop(METER_HEIGHT + 40));
    stereoMeterPk.setBounds(bounds.removeFromRight(105).removeFromTop(METER_HEIGHT + 40));
    //from the bottom of the stereo meter to the bottom of the application height / 2
    
    meterControlColumnL.setBounds(bounds.removeFromLeft(stereoMeterRMS.getWidth() - 5).removeFromTop(stereoMeterRMS.getHeight()));
    
    meterControlColumnR.setBounds(bounds.removeFromRight(stereoMeterRMS.getWidth()- 5).removeFromTop(stereoMeterRMS.getHeight()));
    
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

void Pfmcpp_project10AudioProcessorEditor::takeHoldVal()
{
    juce::String value = meterControlColumnR.holdControl.getText();
    
    if(value == "inf")
    {
        meterControlColumnR.resetHoldButton.setVisible(true);
        
        stereoMeterRMS.setHoldTimeINF(true);
        stereoMeterPk.setHoldTimeINF(true);
        
        return;
    }
    else
    {
        meterControlColumnR.resetHoldButton.setVisible(false);
        stereoMeterRMS.setHoldTimeINF(false);
        stereoMeterPk.setHoldTimeINF(false);
    }
        
    float valInSeconds = value.dropLastCharacters(1).getFloatValue();
    
    stereoMeterRMS.setHoldTime(valInSeconds);
    stereoMeterPk.setHoldTime(valInSeconds);
}


void Pfmcpp_project10AudioProcessorEditor::setDecayValue()
{
    //        StereoMeter RMS & Peak Decay
    juce::String value = meterControlColumnL.decayRateControl.getText();
    float decay = value.dropLastCharacters(4).getFloatValue();

    stereoMeterRMS.setDecayRate(std::abs(decay));
    stereoMeterPk.setDecayRate(std::abs(decay));
}


