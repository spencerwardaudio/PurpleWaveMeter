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
Pfmcpp_project10AudioProcessorEditor::Pfmcpp_project10AudioProcessorEditor (Pfmcpp_project10AudioProcessor& p): AudioProcessorEditor (&p), processor (p), stereoImageMeter(editorBuffer, p.getSampleRate()), decayLabel ("decayLabel", "DECAY")        
		decayLabel ("decayLabel", "DECAY"),        avgLabel ("avgLabel", "AVG"),
        meterLabel ("meterLabel", "VIEW"),
        scaleLabel ("scaleLabel", "SCALE"),
        histLabel ("histLabel", "HIST")
    : AudioProcessorEditor (&p), processor (p), goniometer(editorBuffer), correlationMeter(editorBuffer, p.getSampleRate()),
: AudioProcessorEditor (&p), processor (p), stereoImageMeter(editorBuffer, p.getSampleRate()), decayLabel ("decayLabel", "DECAY"){
    decayLabel.setFont(20.0);
    decayLabel.setColour (juce::Label::textColourId, juce::Colours::orange);
    addAndMakeVisible(decayLabel);
    
    addAndMakeVisible(decayRateControl);
    decayRateControl.addItem("-3dB/s", 1);
    decayRateControl.addItem("-6dB/s", 2);
    decayRateControl.addItem("-12dB/s", 3);
    decayRateControl.addItem("-24dB/s", 4);
    decayRateControl.addItem("-36dB/s", 5);
    
    avgLabel.setFont(20.0);
    avgLabel.setColour (juce::Label::textColourId, juce::Colours::orange);
    addAndMakeVisible(avgLabel);
    
    addAndMakeVisible(avgControl);
    avgControl.addItem("100ms", 1);
    avgControl.addItem("250ms", 2);
    avgControl.addItem("500ms", 3);
    avgControl.addItem("1000ms", 4);
    avgControl.addItem("2000ms", 5);
    
    meterLabel.setFont(20.0);
    meterLabel.setColour (juce::Label::textColourId, juce::Colours::orange);
    addAndMakeVisible(meterLabel);
    
    addAndMakeVisible(meterControl);
    meterControl.addItem("Both", 1);
    meterControl.addItem("Peak", 2);
    meterControl.addItem("Avg", 3);
    
    scaleLabel.setFont(20.0);
    scaleLabel.setColour (juce::Label::textColourId, juce::Colours::orange);
    addAndMakeVisible(scaleLabel);
    
    scaleControl.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    scaleControl.setTextBoxStyle(Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    scaleControl.setLookAndFeel(&scalerLookAndFeel);
    addAndMakeVisible(scaleControl);
    
    enableHoldButton.setColour(ComboBox::outlineColourId, juce::Colours::orange);
    addAndMakeVisible(enableHoldButton);
    
    addAndMakeVisible(holdControl);
    holdControl.addItem("0s", 1);
    holdControl.addItem("0.5s", 2);
    holdControl.addItem("2s", 3);
    holdControl.addItem("4s", 4);
    holdControl.addItem("6s", 5);
    holdControl.addItem("inf", 6);
    
    histLabel.setFont(20.0);
    histLabel.setColour (juce::Label::textColourId, juce::Colours::orange);
    addAndMakeVisible(histLabel);
    
    addAndMakeVisible(histControl);
    histControl.addItem("Stacked", 1);
    histControl.addItem("Side-by-Side", 2);
    
    editorBuffer.setSize(2, processor.maxBufferSize);
    editorBuffer.clear();
    
    addAndMakeVisible(stereoMeterPk);
    addAndMakeVisible(stereoMeterRMS);
    
    addAndMakeVisible(histogramRMS);
    addAndMakeVisible(histogramPeak);
    
    addAndMakeVisible(stereoImageMeter);
    
    addAndMakeVisible(meterControlColumnL);
    addAndMakeVisible(meterControlColumnR);
    
    addAndMakeVisible(histogramRMS);
    addAndMakeVisible(histogramPeak);
    
    meterControlColumnL.decayRateControl.onChange = [this]()
    {
//        stereoMeterRMS
        juce::String value = meterControlColumnL.decayRateControl.getText();
        float decay = value.dropLastCharacters(4).getFloatValue();
        DBG("selected" << value);
        
        stereoMeterRMS.setDecayRate(std::abs(decay));
        stereoMeterPk.setDecayRate(std::abs(decay));
    };
    
    meterControlColumnR.histControl.onChange = [this]()
    {
        auto ID = meterControlColumnR.histControl.getSelectedId();
        
        if(ID == 1)
        {
            stack();
        }
        else if (ID == 2)
        {
            setSideBySide();
        }
    };
    
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
    
    setSize (650, 500);
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
    
}

void Pfmcpp_project10AudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    stereoMeterRMS.setBounds(bounds.removeFromLeft(100).removeFromTop(METER_HEIGHT + 40));
    stereoMeterPk.setBounds(bounds.removeFromRight(100).removeFromTop(METER_HEIGHT + 40));
    //from the bottom of the stereo meter to the bottom of the application height / 2
    stereoImageMeter.setBounds(stereoMeterRMS.getWidth(), 0, bounds.getWidth(), stereoMeterRMS.getHeight());
    histogramContainer.setBounds(0, stereoMeterRMS.getHeight(), getLocalBounds().getWidth(), getLocalBounds().getHeight() - stereoMeterRMS.getHeight());
    
    meterControlColumnL.setBounds(stereoMeterRMS.getRight(), 0, goniometer.getX() - stereoMeterRMS.getRight(), stereoMeterRMS.getHeight());
    meterControlColumnR.setBounds(goniometer.getRight(), 0, stereoMeterPk.getX() - goniometer.getRight(), stereoMeterRMS.getHeight());
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
