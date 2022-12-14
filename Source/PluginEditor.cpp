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
PurpleWaveMeterAudioProcessorEditor::PurpleWaveMeterAudioProcessorEditor (PurpleWaveMeterAudioProcessor& p)
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
    
    meterControlColumnL.decayRateControl.getSelectedIdAsValue().referTo(processor.valueTree.getPropertyAsValue("decayRate", nullptr));

    int i = processor.valueTree.getPropertyAsValue("decayRate", nullptr).getValue();

    if(i == 0)
        i = 1;

    meterControlColumnL.decayRateControl.setSelectedId(i);
    
    meterControlColumnL.avgControl.getSelectedIdAsValue().referTo(processor.valueTree.getPropertyAsValue("averageTime", nullptr, true));
    
    i = processor.valueTree.getPropertyAsValue("averageTime", nullptr).getValue();

    if(i == 0)
        i = 1;

    meterControlColumnL.avgControl.setSelectedId(i);
    
     meterControlColumnL.meterControl.getSelectedIdAsValue().referTo(processor.valueTree.getPropertyAsValue("meterView", nullptr, true));

    i = processor.valueTree.getPropertyAsValue("meterView", nullptr).getValue();

    if(i == 0)
        i = 1;

    meterControlColumnL.meterControl.setSelectedId(i);
    
    meterControlColumnR.scaleControl.getValueObject().referTo(processor.valueTree.getPropertyAsValue("goniometerScale", nullptr, true));

    i = processor.valueTree.getPropertyAsValue("goniometerScale", nullptr).getValue();

    meterControlColumnR.scaleControl.setValue(i);
    
    meterControlColumnR.enableHoldButton.getToggleStateValue().referTo(processor.valueTree.getPropertyAsValue("enableHold", nullptr));
    
    meterControlColumnR.holdControl.getSelectedIdAsValue().referTo(processor.valueTree.getPropertyAsValue("holdTime", nullptr, true));

   i = processor.valueTree.getPropertyAsValue("holdTime", nullptr).getValue();

   if(i == 0)
       i = 1;

    meterControlColumnR.holdControl.setSelectedId(i);
    
    meterControlColumnR.histControl.getSelectedIdAsValue().referTo(processor.valueTree.getPropertyAsValue("histogramView", nullptr, true));
    
    i = processor.valueTree.getPropertyAsValue("histogramView", nullptr).getValue();

    if(i == 0)
        i = 1;

     meterControlColumnR.histControl.setSelectedId(i);
    
    stereoMeterPk.thresholdSlider.getValueObject().referTo(processor.valueTree.getPropertyAsValue("peakThreshold", nullptr, true));

    double z = processor.valueTree.getPropertyAsValue("peakThreshold", nullptr).getValue();

    stereoMeterPk.thresholdSlider.setValue(z);
    
    stereoMeterRMS.thresholdSlider.getValueObject().referTo(processor.valueTree.getPropertyAsValue("rmsThreshold", nullptr, true));

    z = processor.valueTree.getPropertyAsValue("rmsThreshold", nullptr).getValue();

    stereoMeterRMS.thresholdSlider.setValue(z);
    
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
        
//        DBG(processor.valueTree.toXmlString());
    };
    
    meterControlColumnL.meterControl.onChange = [this]()
    {
        auto ID = meterControlColumnL.meterControl.getSelectedId();
    
        stereoMeterRMS.setMeterVisibility(ID);
        stereoMeterPk.setMeterVisibility(ID);
        
//        DBG(processor.valueTree.toXmlString());
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
    
    meterControlColumnR.enableHoldButton.onClick = [this]()
    {
        stereoMeterPk.displayTick();
        stereoMeterRMS.displayTick();
            
        if(!meterControlColumnR.holdButtonOn)
        {
            processor.valueTree.setProperty(processor.enableHold, 1, nullptr);

            meterControlColumnR.holdControl.setVisible(true);
            
            takeHoldVal();
                
            meterControlColumnR.holdButtonOn = true;
        }
        else
        {
            processor.valueTree.setProperty(processor.enableHold, 0, nullptr);
            meterControlColumnR.holdControl.setVisible(false);

            meterControlColumnR.resetHoldButton.setVisible(false);

            meterControlColumnR.holdButtonOn = false;
            
            //set hold time to zero
            stereoMeterRMS.setHoldTime(0);
            stereoMeterPk.setHoldTime(0);

            stereoMeterRMS.setHoldTimeINF(false);
            stereoMeterPk.setHoldTimeINF(false);
        }
    };
    
    if((processor.valueTree.getPropertyAsValue(processor.enableHold, nullptr).getValue().toString()) == "1")
    {
        meterControlColumnR.enableHoldButton.setTriggeredOnMouseDown(true);
        meterControlColumnR.holdControl.setVisible(true);
    }
    else if((processor.valueTree.getPropertyAsValue(processor.enableHold, nullptr).getValue().toString()) == "0")
    {
        meterControlColumnR.holdControl.setVisible(false);
    }
    
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
        histogramContainer.setRMSThreshold(newThreshold);
        
        //update the macrometersRMS
        stereoMeterRMS.setThreshold(newThreshold);
    };
    
    stereoMeterPk.thresholdSlider.onValueChange = [this]()
    {        
        const auto newThreshold = stereoMeterPk.thresholdSlider.getValue();

        //update the histogramPk
        histogramContainer.setPKThreshold(newThreshold);
        
        //update the macrometersPk
        stereoMeterPk.setThreshold(newThreshold);
    };
    
    setSize (650, 500);
    startTimerHz(TimerHz);
}

PurpleWaveMeterAudioProcessorEditor::~PurpleWaveMeterAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
    stopTimer();
}

//==============================================================================
void PurpleWaveMeterAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setColour(Colours::lightskyblue);
    auto bounds = getLocalBounds();
    g.fillRect(bounds);
}

void PurpleWaveMeterAudioProcessorEditor::resized()
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

void PurpleWaveMeterAudioProcessorEditor::timerCallback()
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

void PurpleWaveMeterAudioProcessorEditor::takeHoldVal()
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


void PurpleWaveMeterAudioProcessorEditor::setDecayValue()
{
    //        StereoMeter RMS & Peak Decay
    juce::String value = meterControlColumnL.decayRateControl.getText();
    float decay = value.dropLastCharacters(4).getFloatValue();

    stereoMeterRMS.setDecayRate(std::abs(decay));
    stereoMeterPk.setDecayRate(std::abs(decay));
}


