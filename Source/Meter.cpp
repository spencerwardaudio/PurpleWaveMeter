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
    g.drawImage(image, getLocalBounds().toFloat());
}

void DBScale::resized()
{
    w = getLocalBounds().getWidth();
    h = getLocalBounds().getHeight();
    center = Point<int>(getLocalBounds().getCentre());

    image = Image(Image::RGB, w, h, true);

    Graphics backgroundGraphic { image };

    drawDBBackground(backgroundGraphic);
}

void DBScale::drawDBBackground(Graphics& g)
{
    g.setColour(Colours::white);

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
    
    const auto h = bounds.getHeight();
    const auto w = bounds.getWidth();
    
    auto level = jmap(audioPassingVal, NEGATIVE_INFINITY, MAX_DECIBELS, 0.0f, 1.0f);
    
    const auto meterYPos = h * (1.0 - level);
    
    g.drawImage(image, 0, meterYPos, w, h   , 0, meterYPos, w, h);
    
    //tick meter
    
    level = decayingValueHolder.getCurrentValue();
    
    if(ticksVisible)
    {
        auto tickLine = jmap(level, NEGATIVE_INFINITY, MAX_DECIBELS, 0.0f, 1.0f);

        g.fillRect(bounds.withY(h * (1 -  (float)tickLine)).withHeight(2));
    }
    
    if(decayingValueHolder.isOverThreshold())
    {
        g.setColour(Colours::red.darker().withAlpha(0.5f));
        
        auto h = jmap(decayingValueHolder.getThreshold(), MAX_DECIBELS, NEGATIVE_INFINITY, 0.f, (float)getLocalBounds().getHeight());
        
        Rectangle<float> r(0.f, 0.f, getLocalBounds().getWidth(), h);
        
        g.fillRect(r);
    }
}

void Meter::resized()
{
    const auto h = getHeight();
    const auto w = getWidth();
    
    ticks.clear();
    Tick tck;
    
    for(int i = (int)NEGATIVE_INFINITY; i <= (int)MAX_DECIBELS; i += 6)
    {
        tck.y = jmap(i, (int)NEGATIVE_INFINITY, (int)MAX_DECIBELS, h, 0) + 4;
        tck.dB = i;
        
        ticks.push_back(tck);
    }
    
    image = Image(Image::RGB, w, h, true);
    
    Graphics backgroundGraphic { image };

    drawMeterGradient(backgroundGraphic, getLocalBounds());
}

void Meter::drawMeterGradient(Graphics& g, Rectangle<int> bounds)
{
    g.setOpacity(0.28);
    
    g.setGradientFill (ColourGradient (Colours::lightblue, 0,  0,
                                        Colours::blueviolet, 0, bounds.getHeight(), false));

    g.fillRect(bounds);
}
