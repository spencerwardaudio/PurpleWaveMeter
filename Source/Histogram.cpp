
#include "Histogram.h"


Histogram::Histogram(const String& title_) : title( title_ )
{
    addAndMakeVisible(dBScale);
}
    
void Histogram::paint(Graphics& g)
{
    g.drawImage(imagebackground, getLocalBounds().toFloat());
    
    displayPath(g, getBounds().toFloat());
    
    if(dBLevelClip)
    {
        g.setColour(Colours::red.darker().withAlpha(0.5f));
        auto h = jmap(threshold, MAX_DECIBELS, NEGATIVE_INFINITY, 0.f, (float)getLocalBounds().getHeight());
        
        Rectangle<float> r(0.f, 0.f, getLocalBounds().getWidth(), h);
        
        g.fillRect(r);
        
        dBLevelClip = false;
    }
}

void Histogram::resized()
{
    const auto h = getHeight();
    const auto w = getWidth();
    
    buffer.resize(w, NEG_INF);
    buffer.clear(NEG_INF);
    
    dBScale.yOffset = h;

    dBScale.setBounds(getX(),
                      getY(),
                      h,
                      w);
    
    dBScale.ticks.clear();
    Tick tck;
    
    for(int i = (int)NEGATIVE_INFINITY; i <= (int)MAX_DECIBELS; i += 6)
    {
        tck.y = jmap(i, (int)NEGATIVE_INFINITY, (int)MAX_DECIBELS, h, 0) + 4;
        tck.dB = i;
        
        dBScale.ticks.push_back(tck);
    }
    
    imagebackground = Image(Image::RGB, w, h, true);
    
    Graphics backgroundGraphic { imagebackground };
    
    drawBackground(backgroundGraphic);
}

void Histogram::mouseDown(const MouseEvent& e)
{
    buffer.clear(NEG_INF);
    repaint();
}

void Histogram::update(float value)
{
    buffer.write(value);
    
    if(threshold < value)
    {
        dBLevelClip = true;
    }
    
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
        return juce::jmap(db, NEG_INF, MAX_DB, b.getHeight(), 0.f);
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

void Histogram::setThreshold(float threshAsDecibels)
{
    threshold = threshAsDecibels;
}

void Histogram::drawBackground(Graphics& g)
{
    g.setColour(Colours::black);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0f);
    
    g.setColour(Colours::orange);
    g.drawText(title, getLocalBounds().removeFromBottom(40), Justification::centredBottom);
    
    const auto yOffset = 5;
    const auto lineMargin = 0.075;
    
    auto numTicks = dBScale.ticks.size();
    
    g.setColour (juce::Colours::dimgrey);
    
    for( int i = 0; i < numTicks; ++i )
    {
        if( i % 2 == 0 )
        {
            g.drawSingleLineText(juce::String(dBScale.ticks[i].dB), 0, dBScale.ticks[i].y + yOffset, Justification::left);
            g.drawSingleLineText(juce::String(dBScale.ticks[i].dB), getWidth(), dBScale.ticks[i].y + yOffset, Justification::right);
            g.drawHorizontalLine(dBScale.ticks[i].y, getWidth() * lineMargin, getWidth() - (getWidth() * lineMargin));
        }
    }
}


