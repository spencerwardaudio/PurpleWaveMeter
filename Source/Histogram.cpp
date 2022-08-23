/*
  ==============================================================================

    Histogram.cpp
    Created: 21 Aug 2022 9:08:31pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "Histogram.h"


Histogram::Histogram(const String& title_) : title( title_ )
{
    
}
    
void Histogram::paint(Graphics& g)
{
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());
    
    g.setColour(Colours::mintcream);
    g.drawText(title, getLocalBounds().removeFromTop(40).removeFromLeft(100), Justification::centredBottom);
    
    displayPath(g, getBounds().toFloat());
    
}

void Histogram::resized()
{
    buffer.resize(getWidth(), NEG_INF);
}

void Histogram::mouseDown(const MouseEvent& e)
{
    buffer.clear(NEG_INF);
    repaint();
}

void Histogram::update(float value)
{
    buffer.write(value);
    repaint();
}

void Histogram::displayPath(Graphics& g, Rectangle<float> bounds)
{
    auto fill = buildPath(path, buffer, getBounds().toFloat());

    if(!fill.isEmpty())
    {
        g.fillAll(Colours::purple.withAlpha(0.5f));
        g.strokePath(fill, PathStrokeType(1));
    }
}

Path Histogram::buildPath(Path& p,
                      ReadAllAfterWriteCircularBuffer<float>& buffer,
                      Rectangle<float> bounds)
{
    p.clear();

    auto size = buffer.getSize();
    auto b = bounds;
    auto& data = buffer.getData();
    auto readIndex = buffer.getReadIndex();

    auto map = [&b] (float db)
    {
        return juce::jmap(db, NEG_INF, MAX_DB, b.getBottom(), 0.f);
    };

    auto increment = [&readIndex, &size] (size_t index)
    {
        readIndex++;

        if(readIndex > size - 1)
        {
            readIndex = 0;
        }
    };

    p.startNewSubPath(0, map(data[readIndex]));

    increment(readIndex);

    for(int x = 1; x < b.getWidth(); x++)
    {
        p.lineTo(x, map(data[readIndex]));
        
        jassert(map(data[readIndex]) > 0);

        increment(readIndex);
    }

//    p.closeSubPath();

    //????  if the bounds of the path has a height greater than 0
//    if(map(data[readIndex]) > 0)
//    {
//        auto fillCopy = fill;

//        fill.drawLine(bounds.removeFromRight(1), 1);
//        g.drawLine(bounds.removeFromLeft(1), 1);

//        fill.closeSubPath();

//        return map(data[readIndex]);
//    }
//    Path newPath;
//    newPath.clear();

    return p;
}
