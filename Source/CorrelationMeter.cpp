/*
  ==============================================================================

    CorrelationMeter.cpp
    Created: 8 Sep 2022 10:12:40pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "CorrelationMeter.h"

CorrelationMeter::CorrelationMeter(AudioBuffer<float>& buf, double sampleRate) : buffer(buf)
{
    filters = { FilterType(dsp::FilterDesign<float>::designFIRLowpassWindowMethod(100, sampleRate, 2, juce::dsp::WindowingFunction<float>::blackmanHarris)), FilterType(dsp::FilterDesign<float>::designFIRLowpassWindowMethod(100, sampleRate, 2, juce::dsp::WindowingFunction<float>::blackmanHarris)), FilterType(dsp::FilterDesign<float>::designFIRLowpassWindowMethod(100, sampleRate, 2, juce::dsp::WindowingFunction<float>::blackmanHarris)) };
    
    spec.sampleRate = sampleRate;
    spec.numChannels = 1;
    spec.maximumBlockSize = buffer.getNumSamples();
    
    //just prepare for individual channels  per filter
    for (auto& i : filters)
    {
        i.prepare(spec);
        i.reset();
    }
}

void CorrelationMeter::paint(Graphics& g)
{
    Rectangle<float> r = Rectangle<float> (getLocalBounds().toFloat());
    
    drawAverage(g, getLocalBounds().removeFromBottom(15), slowAverager.getAverage(), true);
    drawAverage(g, getLocalBounds().removeFromTop(5), peakAverager.getAverage(), true);
    
    g.setColour (Colours::red);
    g.drawRect(r);
}

void CorrelationMeter::update()
{
    float result = 0;
    
    //compute the per buffer average correlation
    for(int j = 0; j < buffer.getNumSamples(); j++)
    {
        float LT = buffer.getSample(0, j);
        float LTSquared = LT * LT;
        float RT = buffer.getSample(1, j);
        float RTSquared = RT * RT;
        float LTRT = LT * RT;
        
        auto H0 = filters[0].processSample(LTRT);
        auto H1 = filters[1].processSample(LTSquared);
        auto H2 = filters[2].processSample(RTSquared);
        
        result = H0 / (std::sqrt(H1 * H2));
        
        if(isnan(result))
            result = 0;
     
        slowAverager.add(result);
        peakAverager.add(result);
    }
    
    repaint();
}

void CorrelationMeter::drawAverage(Graphics& g,
                 Rectangle<int> bounds,
                 float avg,
                 bool drawBorder)
{
    DBG("avg: " << avg);
    g.setColour ( Colours::yellowgreen );
    
    auto center = (float)bounds.getWidth()/2;
    
    float mappedVal = jmap(avg, -1.f, 1.0f, (float)bounds.getX(), (float)bounds.getWidth());
    
    g.setColour(Colours::greenyellow.withAlpha(0.8f));
    
    if(mappedVal > 1.0f)
        mappedVal = (float)bounds.getWidth();
    
    if((mappedVal - center) > 0.0f)
    {
        g.fillRect(center, (float)bounds.getY(), mappedVal - center, (float)bounds.getHeight());
        g.drawRect(center, (float)bounds.getY(), mappedVal - center, (float)bounds.getHeight(), 1.f);
    }
    else if((mappedVal - center) < 0.0f)
    {
        g.fillRect(mappedVal, (float)bounds.getY(), center, (float)bounds.getHeight());
        g.drawRect(mappedVal, (float)bounds.getY(), center, (float)bounds.getHeight(), 1.f);
    }
    else
    {
        g.fillRect(center, (float)bounds.getY(), 1.0f, (float)bounds.getHeight());
    }
        
}
