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
    auto coefficients = dsp::FilterDesign<float>::designFIRLowpassWindowMethod(50.f, sampleRate, 2, dsp::FilterDesign<float>::WindowingMethod::rectangular);
    
    filters[0] = dsp::FIR::Filter<float>(coefficients);
    filters[1] = dsp::FIR::Filter<float>(coefficients);
    filters[2] = dsp::FIR::Filter<float>(coefficients);
    
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
        
        if(H1 == 0 || H2 == 0)
            result = 0;
        
        if(isnan(result) || isinf(result))
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
    g.setColour ( Colours::yellowgreen );
    
    float mappedVal = jmap(avg, -1.f, 1.0f, 0.f
                           , (float)bounds.getWidth());
    
    g.setColour(Colours::greenyellow.withAlpha(0.8f));
    
    Rectangle<float> rN(mappedVal, 0.f, getLocalBounds().getWidth()/2, getLocalBounds().getHeight());
    Rectangle<float> rP(getLocalBounds().getWidth()/2, 0.f, (mappedVal - getLocalBounds().getWidth()/2), getLocalBounds().getHeight());
    
    if(avg < 0.f)
        g.fillRect(rN);
    else if (avg >= 0.f)
        g.fillRect(rP);
}
