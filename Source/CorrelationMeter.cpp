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
    auto bounds = getLocalBounds();
    auto leftLabelArea = bounds.removeFromLeft(25);
    auto rightLabelArea = bounds.removeFromRight(25);
    
    g.setColour(Colours::black);
    g.fillRect(getLocalBounds());
    
    g.setColour(Colours::whitesmoke.withAlpha(0.7f));
    
    g.drawText("-1", leftLabelArea, Justification::centred);
    g.drawText("+1", rightLabelArea, Justification::centred);
    
    auto slowMeterArea = bounds.removeFromTop(15);
    auto peakMeterArea = bounds.removeFromTop(5);
    
    drawAverage(g, slowMeterArea, slowAverager.getAverage(), true);
    drawAverage(g, peakMeterArea, peakAverager.getAverage(), true);
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
        
        if(H1 == 0 || H2 == 0)
        {
            result = 0;
        }
        else
        {
            result = H0 / (std::sqrt(H1 * H2));
        }
        
        if(isnan(result) || isinf(result))
        {
            result = 0;
        }

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
    float mappedVal = jmap(avg, -1.f, 1.0f, 0.0f, 1.0f);
    
    auto centerPoint = bounds.getRelativePoint(0.5f, 0.f);
    auto correlationPoint = bounds.getRelativePoint(mappedVal, 1.0f);
    auto r = juce::Rectangle<int>(centerPoint, correlationPoint);
    
    if(mappedVal <= 0.f)
    {
        g.setGradientFill (ColourGradient (Colours::whitesmoke, bounds.getX(), 0,
                                            Colours::blueviolet, bounds.getCentreX(), getHeight(), false));
    }
    else
    {
        g.setGradientFill (ColourGradient (Colours::blueviolet, bounds.getCentreX(), 0,
                                            Colours::whitesmoke, bounds.getWidth(), getHeight(), false));
    }
    
    g.fillRect(r);
}
