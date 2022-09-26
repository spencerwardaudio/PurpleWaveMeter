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
    Rectangle<int> rSA = Rectangle<int> (25, 5, getLocalBounds().getWidth() - 50, getLocalBounds().getHeight() - 5);
    Rectangle<int> rPA = Rectangle<int> (25, 0, getLocalBounds().getWidth() - 50, getLocalBounds().getHeight() - 15);
    
    juce::String labels[2] { "-1", "+1"};
    
    g.setColour(Colours::whitesmoke.withAlpha(0.7f));
    
    g.drawText(labels[0], 0, 0, 20, 20, Justification::centred);
    g.drawText(labels[1], getWidth() - 20, 0, 20, 20, Justification::centred);
    
    drawAverage(g, rSA, slowAverager.getAverage(), true);
    drawAverage(g, rPA, peakAverager.getAverage(), true);
    
    g.setColour (Colours::red);
//    g.drawRect(rSA, 1);
//    g.drawRect(rPA, 1);
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
//    avg = 0.95f;
    
    float mappedVal = jmap(avg, -1.f, 1.0f, 0.f
                           , (float)bounds.getWidth());
    
    Rectangle<float> rN(mappedVal + 25, bounds.getY(), bounds.getWidth()/2 - mappedVal, bounds.getHeight());
    Rectangle<float> rP(bounds.getWidth()/2 + 25, bounds.getY(), (mappedVal - bounds.getWidth()/2), bounds.getHeight());
    
    if(avg < 0.f)
    {
        g.setGradientFill (ColourGradient (Colours::whitesmoke, 25, 0,
                                            Colours::blueviolet, bounds.getWidth()/2, 0, false));
        g.fillRect(rN);
    }
    else if(avg >= 0.f)
    {
        g.setGradientFill (ColourGradient (Colours::blueviolet, bounds.getWidth()/2 + 25, 0,
                                            Colours::whitesmoke, bounds.getWidth(), 0, false));
        g.fillRect(rP);
    }
        
}
