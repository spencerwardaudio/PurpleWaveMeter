/*
  ==============================================================================

    MacroMeter.cpp
    Created: 8 Jun 2021 9:44:17am
    Author:  Spencer Ward

  ==============================================================================
*/

#include "MacroMeter.h"


MacroMeter::MacroMeter(int insMtrX, int avgMtrX)
{
    meterIPos = insMtrX;
    meterAVGPos = avgMtrX;
    
    addAndMakeVisible(meterInstant);
    addAndMakeVisible(meterAverage);
    
    addAndMakeVisible(textMeter);
    
    meterInstant.decayingValueHolder.setHoldTime(1000);
    meterInstant.decayingValueHolder.setDecayRate(3.f);
    
    meterAverage.decayingValueHolder.setHoldTime(1000);
    meterAverage.decayingValueHolder.setDecayRate(3.f);
}

void MacroMeter::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::black);

}

void MacroMeter::resized()
{
    meterInstant.setBounds(meterIPos,
                           10,
                           METER_WIDTH,
                           METER_HEIGHT);

    meterAverage.setBounds(meterAVGPos,
                           10,
                           METER_WIDTH / 3,
                           METER_HEIGHT);

    textMeter.setBounds(0,
                        -27,
                        40,
                        41);
    
}

void MacroMeter::update(float levelInDB)
{
    meterInstant.update(levelInDB);
    
    textMeter.update(levelInDB);
    
    averageValue.add(levelInDB);
    
    auto avg = averageValue.getAverage();
    
    meterAverage.update(avg);
}

std::vector<Tick> MacroMeter::getDBTick()
{
    return meterInstant.ticks;
}

int MacroMeter::getDBBounds()
{
    return meterInstant.getY();
}

void MacroMeter::setThreshold(float threshAsDecibels)
{
    threshold = threshAsDecibels;
    meterInstant.setThreshold(threshAsDecibels);
    meterAverage.setThreshold(threshAsDecibels);
}
