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
    
    g.drawText(title, getLocalBounds().removeFromBottom(40), Justification::centredBottom);
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
    
    auto colors = std::vector<Colour>
    {
        Colours::violet.withAlpha(0.8f),
        Colours::blue.withAlpha(0.8f),
        Colours::whitesmoke.withAlpha(0.8f),
    };
    
    ColourGradient cg;
    
    for(int i = 0; i < colors.size(); ++i)
    {
        cg.addColour((double(i) / double(colors.size() - 1)), colors[i]);
    }

    
    if(!fill.isEmpty())
    {
        cg.point1 = {0, (float)getHeight()};
        cg.point2 = {0, 0};
        g.setGradientFill(cg);
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
        return juce::jmap(db, NEG_INF, MAX_DB, (float)HISTOGRAM_HEIGHT, 0.f);
    };

    auto increment = [] (size_t &index, size_t &bufferSize)
    {
        index++;

        if(index > bufferSize - 1)
        {
            index = 0;
        }
    };

    p.startNewSubPath(0, map(data[readIndex]));

    increment(readIndex, size);

    for(int x = 1; x < b.getWidth(); x++)
    {
        p.lineTo(x, map(data[readIndex]));
        
        //colour in gradient
        p.lineTo(x, b.getBottom());

        increment(readIndex, size);
    }

    if( p.getBounds().getHeight() > 0 )
    {
        auto pathDuplicate = p;

        pathDuplicate.lineTo(b.getBottomRight());
        pathDuplicate.lineTo(b.getBottomLeft());
        
        pathDuplicate.closeSubPath();
        
        return pathDuplicate;
    }

    return p;
}
