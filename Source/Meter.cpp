/*
  ==============================================================================

    Meter.cpp
    Created: 14 May 2021 7:25:43am
    Author:  Spencer Ward

  ==============================================================================
*/

#include "Meter.h"


void TextMeter::update(float audioValue)
{
    level = Decibels::gainToDecibels(audioValue);
    
    valueHolder.updateHeldValue(level);
    decayingValueHolder.updateHeldValue(level);
    
    repaint();
}

void TextMeter::paint(Graphics& g)
{
    String str;
    
    auto over = valueHolder.isOverThreshold();
    if( over )
    {
        str = juce::String( valueHolder.getCurrentValue(), 1 );
    }
    else
    {
        str = ( level <= NegativeInfinity ) ? "-inf" : juce::String( level, 1 );
    }
    
    g.fillAll ( over ? Colours::red : Colours::black );
    g.setColour ( Colours::white );
    g.drawSingleLineText(str, 5, Justification::centred);
}

void DBScale::paint(Graphics& g)
{
    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    
    Rectangle<int> r;
    r.setWidth(getWidth());
    r.setHeight(14);
    r.setX(0);
    r.setY(0);

    for(auto t : ticks)
    {
        g.drawSingleLineText(juce::String(t.dB), 0, t.y + yOffset);
    }
}

void Meter::update(float audioValue)
{
    audioPassingVal = Decibels::gainToDecibels(audioValue);
    repaint();
}

void Meter::paint(Graphics& g)
{
    g.fillAll(Colours::white);
    
    auto bounds = getLocalBounds();

    for(auto& tck : ticks )
    {
        g.setColour (juce::Colours::black);
        
        juce::Line<float> line (juce::Point<float> (2, tck.y - 3),
                                juce::Point<float> (37, tck.y - 3));

        g.drawLine (line, 0.8f);
    }
    
    auto h = bounds.getHeight();
    auto level = jmap((double)audioPassingVal, NegativeInfinity, MaxDecibels, 0.0, 1.0);
    
    g.setColour(Colours::greenyellow);
    g.fillRect(bounds.withHeight(h * level).withY(h * (1.0 - level)));
}

void Meter::resized()
{
    auto h = getHeight();
    
    ticks.clear();
    Tick tck;
    
    for(int i = (int)NegativeInfinity; i <= (int)MaxDecibels; i += 6)
    {
        tck.y = jmap(i, (int)NegativeInfinity, (int)MaxDecibels, h, 0) + 4;
        std::cout << tck.y << " : y " << std::endl;
        tck.dB = i;
        std::cout << tck.dB << " : dB " << std::endl;
        
        ticks.push_back(tck);
    }
}