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

    g.setColour (Colours::black);
    g.setFont (15.0f);
    
    Rectangle<float> r(stereoMeterRMS.getRight(), 0, goniometer.getX() - stereoMeterRMS.getRight(), histogramRMS.getY());
    g.fillRect(r);
    
    r = Rectangle<float> (goniometer.getRight(), 0, stereoMeterPk.getRight() - goniometer.getRight(), histogramRMS.getY());
    g.fillRect(r);
    
    //Combo Box GUI Dividers LEFT
    g.setColour (Colours::white);
    
    g.drawLine(stereoMeterRMS.getRight(), decayRateControl.getBottom() + 10, stereoMeterRMS.getWidth() + decayRateControl.getWidth(), decayRateControl.getBottom() + 10, 1);
    
    g.drawLine(stereoMeterRMS.getRight(), avgControl.getBottom() + 10, stereoMeterRMS.getWidth() + decayRateControl.getWidth(), avgControl.getBottom() + 10, 1);
    
    g.drawLine(stereoMeterRMS.getRight(), meterControl.getBottom() + 10, stereoMeterRMS.getWidth() + decayRateControl.getWidth(), meterControl.getBottom() + 10, 1);
    
    //Combo Box GUI Dividers RIGHT
    g.drawLine(goniometer.getRight(), scaleControl.getBottom() + 10, scaleControl.getWidth() + goniometer.getRight(), scaleControl.getBottom() + 10, 1);
    
    g.drawLine(goniometer.getRight(), holdControl.getBottom() + 10, scaleControl.getWidth() + goniometer.getRight(), holdControl.getBottom() + 10, 1);
    
    g.drawLine(goniometer.getRight(), histControl.getBottom() + 10, scaleControl.getWidth() + goniometer.getRight(), histControl.getBottom() + 10, 1);
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
    decayRateControl.setBounds(stereoMeterRMS.getWidth(), decayLabel.getHeight(), 50, 25);
    decayLabel.setBounds(stereoMeterRMS.getWidth(), 0, 50, 25);
    avgLabel.setBounds(stereoMeterRMS.getWidth(), 0, 50, 25);;
    avgControl.setBounds(stereoMeterRMS.getWidth(), decayLabel.getHeight(), 50, 25);;
    meterLabel.setBounds(stereoMeterRMS.getWidth(), 0, 50, 25);;
    meterControl.setBounds(stereoMeterRMS.getWidth(), decayLabel.getHeight(), 50, 25);;
    scaleLabel.setBounds(goniometer.getRight() + 35, 15, 50, 25);
    scaleControl.setBounds(goniometer.getRight(), scaleLabel.getBottom(), 70, 70);
    enableHoldButton.setBounds(goniometer.getRight() + 35, scaleControl.getBottom() + 10, 50, 25);
    holdControl.setBounds(goniometer.getRight(), enableHoldButton.getBottom(), 75, 25);
    histLabel.setBounds(goniometer.getRight() + 35, holdControl.getBottom() + 20, 50, 25);
    histControl.setBounds(goniometer.getRight(), histLabel.getBottom(), 75, 25);
    
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
