/*
  ==============================================================================

    Goniometer.cpp
    Created: 26 Aug 2022 4:39:56pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "Goniometer.h"

Goniometer::Goniometer(AudioBuffer<float>& buffer) : _buffer(buffer), image(Image::RGB, 100, 100, true), backgroundGraphic(image)
{
    internalBuffer.clear();
    internalBuffer.setSize(2, 256);
}

void Goniometer::paint(Graphics& g)
{
    drawBackground(backgroundGraphic);
    
    p.clear();
    
    internalBuffer.copyFrom(0, 0, _buffer, 0, 0, internalBuffer.getNumSamples());
    internalBuffer.copyFrom(1, 0, _buffer, 1, 0, internalBuffer.getNumSamples());
    
//    get the left channel sample and right channel sample.
    for (int i = 0; i < 256; i++)
    {
        auto sampleDBL = Decibels::gainToDecibels(internalBuffer.getSample(0, i));
        auto sampleDBR = Decibels::gainToDecibels(internalBuffer.getSample(1, i));
//
        float S = (sampleDBL-sampleDBR) - 3;
        float M = (sampleDBL+sampleDBR) - 3;
        
        Point<float> point(center.toFloat().x, center.toFloat().y);
        
        auto rangedS = jmap(S, -10.f, 0.f, 0.f, (float)w/4.f) + center.x;
        auto rangedM = jmap(M, -100.f, 0.f, 0.f, (float)h/4.f) + center.y;
        
        Point<float> p1(rangedS, rangedM);
        
        if(M <= -100.f)
        {
            return;
        }
        
        if(i == 0)
        {
            p.startNewSubPath(p1);
        }
        else if(p1.isFinite())
        {
            p.lineTo(p1);
        }
        
        if(!p.isEmpty())
        {
            p.applyTransform(AffineTransform::verticalFlip(h));
        }
        
        auto colors = std::vector<Colour>
        {
            Colours::green,
            Colours::blue
        };
        
        ColourGradient gFill;
        
        gFill.isRadial = true;
        
        gFill.point1 = {(float)center.x, (float)center.y };
        gFill.point2 = { (float)(w/4), (float)center.y };
        
        for(int i = 0; i < colors.size(); ++i)
        {
            gFill.addColour((double(i) / double(colors.size() - 1)), colors[i]);
        }

        g.setGradientFill(gFill);
        g.strokePath(p, PathStrokeType(3));
        
        i = i+3;

    }
}

void Goniometer::resized()
{
    w = getLocalBounds().getWidth();
    h = getLocalBounds().getHeight();
    center = Point<int>(w/2, h/2);
    
    image.rescaled(w, h);
}

void Goniometer::drawBackground(Graphics& g)
{
//    Graphics background (image);
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());
    
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
    g.drawImage(image, getLocalBounds().toFloat());
}
