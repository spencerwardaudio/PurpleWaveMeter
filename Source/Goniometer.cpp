/*
  ==============================================================================

    Goniometer.cpp
    Created: 26 Aug 2022 4:39:56pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "Goniometer.h"

Goniometer::Goniometer(AudioBuffer<float>& buffer) : _buffer(buffer), image(Image::RGB, 240, 240, true), backgroundGraphic(image)
{
    internalBuffer.setSize(2, 1024);
    internalBuffer.clear();
}

void Goniometer::paint(Graphics& g)
{
    g.drawImage(image, getLocalBounds().toFloat());
    
    p.clear();
    
    auto numChannels = _buffer.getNumChannels();
    auto eBufNumSamples = _buffer.getNumSamples();
    auto iBufNumSamples = internalBuffer.getNumSamples();
    
    if(iBufNumSamples > eBufNumSamples)
    {
        for (int i  = 0; i < numChannels; ++i)
        {
            auto* readPointer_BBuffer = _buffer.getReadPointer(i);
            auto* readPointer_IBuffer = internalBuffer.getReadPointer(i);
            auto* writePointer_IBuffer = internalBuffer.getWritePointer(i);
            
            //shift elements in the internal buffer based on the size of the editor buffer
            std::copy(readPointer_IBuffer + eBufNumSamples, readPointer_IBuffer + iBufNumSamples, writePointer_IBuffer);
            
            //append elements to the internal buffer based on the size of the editor buffer
            std::copy(readPointer_BBuffer, readPointer_BBuffer + eBufNumSamples, writePointer_IBuffer + eBufNumSamples);
        }
    }
    
    internalBuffer = _buffer;
    
//    get the left channel sample and right channel sample.
    for (int i = 0; i < 256;)
    {
        auto sub3 = Decibels::decibelsToGain(-3.f);

        auto sampleL = internalBuffer.getSample(0, i);
        auto sampleR = internalBuffer.getSample(1, i);

        float S = (sampleL-sampleR) * sub3;
        float M = (sampleL+sampleR) * sub3;
        
        Point<float> point(center.toFloat().x, center.toFloat().y);
        
        auto radius = w/1.3;
        
        auto rangedS = jmap(S, -1.f, 1.f, (float)(center.x - radius), (float)(center.x + radius));
        auto rangedM = jmap(M, -1.f, 1.f, (float)(center.y - radius), (float)(center.y + radius));
        
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

        i = i+3;
    }
    
    if(!p.isEmpty())
    {
        p.applyTransform(AffineTransform::verticalFlip(h));
        
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
        g.strokePath(p, PathStrokeType(1));
    }
}

void Goniometer::resized()
{
    w = getLocalBounds().getWidth();
    h = getLocalBounds().getHeight();
    center = Point<int>(w/2, h/2);
    
    drawBackground(backgroundGraphic);
}

void Goniometer::drawBackground(Graphics& g)
{
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());
    
    g.setColour(Colours::whitesmoke.withAlpha(0.7f));
    
    auto radius = w/1.3;
    g.drawEllipse(getLocalBounds().withSizeKeepingCentre(radius, radius).toFloat(), 1.0f);

    juce::String labels[5] { "+S", "L", "M", "R", "-S" };

    for(int i = 0; i < 8; i++)
    {
        Point<float> radiusPoint = center.getPointOnCircumference(w/2.7, degreesToRadians((i*45.f) - 90.f));
        
        g.drawLine(center.x, center.y, radiusPoint.getX(), radiusPoint.getY());
        
        Point<float> radiusPointOffset = center.getPointOnCircumference(w/2.3, degreesToRadians((i*45.f) - 90.f));
        
        if(i < 5)
            g.drawText(labels[i], radiusPointOffset.x - 7, radiusPointOffset.y - 5, 15, 15, Justification::centredTop);
    }
}
