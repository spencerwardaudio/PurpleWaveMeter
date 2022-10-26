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
    addAndMakeVisible(dBScale);
    //resize the vector of levels based on your width()
    levels.resize(size_t(getWidth()));
}
    
void Histogram::paint(Graphics& g)
{
    g.drawImage(imagebackground, getLocalBounds().toFloat());
    
    g.drawImage(image, getLocalBounds().toFloat());

//    displayPath(g, getBounds().toFloat());
    
    //==============================================================
    
    const auto fHeight = (float)getHeight();
    // draw using an image
    const auto val = 200;
     const auto top = (1.0f-val)*fHeight;
//    Colour col = Colours::orange;
//     const auto cola = col.withAlpha(0.4f);
//
//     for (int y = getHeight(); y > int(std::round(top)); --y) {
//         image.setPixelAt(getWidth()-count, y, image);
//     }

    //draw a bunch of lines from values in a vector. Might be less efficient, but can cope with resizing.
//    g.setColour(col.withAlpha(0.4f));
    
    auto size = buffer.getSize();
    auto b = getLocalBounds().toFloat();
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

//    p.startNewSubPath(0, map(data[readIndex]));
    //this is the first draw point in the path
    

    increment(readIndex, size);

    for(int x = 1; x < b.getWidth(); x++)
    {
//        p.lineTo(x, map(data[readIndex]));
        levels[count] = map(data[readIndex]);
//        //colour in gradient
//        p.lineTo(x, b.getBottom());

        count = (count+1)%getWidth();
        increment(readIndex, size);
    }
    
    //Just for testing
//    levels[count] = val;
//     for (auto&& level : levels) {
    for (size_t i = 0; i < levels.size(); ++i) {
        auto level = levels[i];
//        auto top = (1.0f-level)*fHeight;
//        g.drawVerticalLine(getWidth()-i, level, fHeight);
        g.drawImage(image, getWidth()-i, level, 1, fHeight, 0, 0, 1, level);
        // g.drawVerticalLine(count, fHeight, top);
    }

//    g.drawImage(image, getLocalBounds().toFloat());
    
    //==============================================================

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
    buffer.resize(getWidth(), NEG_INF);
    buffer.clear(NEG_INF);
    
    const auto h = getHeight();
    const auto w = getWidth();
    
    dBScale.yOffset = h;

    dBScale.setBounds(getX(),
                      getY(),
                      getHeight(),
                      getWidth());
    
    dBScale.ticks.clear();
    Tick tck;
    
    for(int i = (int)NEGATIVE_INFINITY; i <= (int)MAX_DECIBELS; i += 6)
    {
        tck.y = jmap(i, (int)NEGATIVE_INFINITY, (int)MAX_DECIBELS, h, 0) + 4;
        std::cout << tck.y << " : y " << std::endl;
        tck.dB = i;
        std::cout << tck.dB << " : dB " << std::endl;
        
        dBScale.ticks.push_back(tck);
    }
    
    image = Image(Image::RGB, w, h, true);

    Graphics gradientRect { image };
    
    drawColourGradientRect(gradientRect, getLocalBounds());
//    pass in the image
    
    imagebackground = Image(Image::RGB, w, h, true);
    
    Graphics backgroundGraphic { imagebackground };
    
    drawBackground(backgroundGraphic);
    
//    fg = juce::Image(juce::Image::PixelFormat::ARGB, getWidth(), getHeight(), true);
    levels.resize(size_t(getWidth()));
//    const auto h = getHeight();
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
    
//    auto fill = buildPath(path, buffer, getBounds().toFloat());
//
//
//    if(!fill.isEmpty())
//    {
//        //draw a column
//        g.drawImage(image, 0, 0, 1, bounds.getHeight(), 0, 0, 1, bounds.getHeight());
//    }
//    for(int i = 0; i < buffer.getSize(); i++)
        g.drawImage(image, 150, 0, 50, bounds.getHeight(), 0, 0, 50, bounds.getHeight());
//    g.drawLine(<#float startX#>, <#float startY#>, <#float endX#>, <#float endY#>)
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
    //this is the first draw point in the path
    

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

void Histogram::drawColourGradientRect(Graphics& g, Rectangle<int> bounds)
{
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

    cg.point1 = { 0, (float)bounds.getHeight() };
    cg.point2 = { 1, 0 };
    g.setGradientFill( cg );
    g.fillRect( 0, 0, 1,  bounds.getHeight() );
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


//First draw a rectangle with a width of 1 with a gradient
//Second for every paint call draw the rectangle with the given level height into the image



