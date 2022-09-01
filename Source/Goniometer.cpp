/*
  ==============================================================================

    Goniometer.cpp
    Created: 26 Aug 2022 4:39:56pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "Goniometer.h"

Goniometer::Goniometer(AudioBuffer<float>& buffer) : buffer(internalBuffer)
{
    internalBuffer.clear();
    internalBuffer.setSize(2, 256);
}

void Goniometer::paint(Graphics& g)
{
    drawBackground(g.drawImage(image, getLocalBounds()));
    
    p.clear();
    
    auto s = buffer.getNumSamples();
    
    //TODO copy samples into the buffer
}

void Goniometer::resized()
{
    w = getLocalBounds().getWidth();
    h = getLocalBounds().getHeight();
    center = Point<int>(w/2, h/2);
    
    image = juce::Image(Image::RGB, w, h, true);

}

void Goniometer::drawBackground(Graphics& g)
{
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());
    
//    //reference outline
//    auto bounds = getLocalBounds();
//    g.setColour(Colours::red);
//    g.drawRect(bounds);
    
    g.setColour(Colours::whitesmoke.withAlpha(0.7f));
    
    auto scale = 1.3;
    auto scaledWidth = w/scale;
    auto e = (w - scaledWidth)/2;
    g.drawEllipse(e, e, w/scale, h/scale, 1.0f);
    
    juce::String labels[5] { "+S", "L", "M", "R", "-S" };
    
    for(int i = 0; i < 8; i++)
    {
        auto x = (scaledWidth/2) * cos((i*MathConstants<float>::pi)/4) + center.x;
        auto y = (scaledWidth/2) * sin((i*MathConstants<float>::pi)/4) + center.y;
        
        g.drawLine(center.x, center.y, x, y);
        
        if(i >= 4)
        {
            x = (scaledWidth/2 + 7) * cos((i*MathConstants<float>::pi)/4) + center.x - 10;
            y = (scaledWidth/2 + 7) * sin((i*MathConstants<float>::pi)/4) + center.y - 10;
            g.drawText(labels[i-4], x, y, 15, 15, Justification::centred);
        }
        else if(i == 0)
        {
            x = ((scaledWidth + 7) /2) * cos((i*MathConstants<float>::pi)/4) + center.x;
            y = ((scaledWidth + 7) /2) * sin((i*MathConstants<float>::pi)/4) + center.y - 10;
            g.drawText(labels[4], x, y, 15, 15, Justification::centred);
        }
    }
}
