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
    valueHolder.updateHeldValue(audioValue);
    
    level = audioValue;
    
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
        str = ( level <= NEGATIVE_INFINITY ) ? "-inf" : juce::String( level, 1 );
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
    decayingValueHolder.updateHeldValue(audioValue);
    
    audioPassingVal = audioValue;
    
    repaint();
}

void Meter::paint(Graphics& g)
{
    g.fillAll(Colours::black);
    
    auto bounds = getLocalBounds();
    
    auto xOffset = 2;
    
    auto yOffset = -4;
    
    auto center = bounds.getCentreX();

    auto numTicks = ticks.size();
    for( int i = 0; i < numTicks; ++i )
    {
        g.setColour (juce::Colours::whitesmoke);
        
        if(i % 2 == 0 ? xOffset = 5 : xOffset = 11)
        {
            g.drawHorizontalLine(ticks[i].y + yOffset, center - xOffset, center + xOffset);
        }
    }
    
    auto h = bounds.getHeight();
    auto level = jmap(audioPassingVal, NEGATIVE_INFINITY, MAX_DECIBELS, 0.0f, 1.0f);
    
    g.setColour(Colours::steelblue);
    
    g.setOpacity(0.28);
    
    g.setGradientFill (ColourGradient (Colours::lightblue, 0,  level,
                                        Colours::blueviolet, 0, h, false));

    g.fillRect(bounds.withHeight(h * level).withY(h * (1.0 - level)));
    
    //tick meter
    
    level = decayingValueHolder.getCurrentValue();
    
    auto tickLine = jmap(level, NEGATIVE_INFINITY, MAX_DECIBELS, 0.0f, 1.0f);

    g.fillRect(bounds.withY(h * (1 -  (float)tickLine)).withHeight(2));
 
    if(decayingValueHolder.isOverThreshold())
    {
        g.setColour(Colours::red.darker().withAlpha(0.5f));
        
        auto h = jmap(decayingValueHolder.getThreshold(), MAX_DECIBELS, NEGATIVE_INFINITY, 0.f, (float)getLocalBounds().getHeight());
        
        Rectangle<float> r(0.f, 0.f, getLocalBounds().getWidth(), h);
        
        DBG("newThreshold" << h);
        
        g.fillRect(r);
    }
}

void Meter::resized()
{
    auto h = getHeight();
    
    ticks.clear();
    Tick tck;
    
    for(int i = (int)NEGATIVE_INFINITY; i <= (int)MAX_DECIBELS; i += 6)
    {
        tck.y = jmap(i, (int)NEGATIVE_INFINITY, (int)MAX_DECIBELS, h, 0) + 4;
        std::cout << tck.y << " : y " << std::endl;
        tck.dB = i;
        std::cout << tck.dB << " : dB " << std::endl;
        
        ticks.push_back(tck);
    }
}
