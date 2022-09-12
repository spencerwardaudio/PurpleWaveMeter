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
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    spec.maximumBlockSize = buffer.getNumSamples();
    
    Array<dsp::FIR::Coefficients<float>> firLowPassCoefficients = dsp::FilterDesign<float>::designFIRLowpassWindowMethod(100, sampleRate, 2, Window);
    
//    for (auto i : filters)
//    {
//        i(firLowPassCoefficients);
//        i.prepare(spec);
//        i.
//        i.reset();
//        i.setCutoffFrequency(100);
//    }
    
    for (int i = 0; i < 3; i++)
    {
        new FilterType(firLowPassCoefficients);
    }
}

// all of the time goes to actually building 
void CorrelationMeter::paint(Graphics& g)
{
    //    drawAverage();
    //    drawAverage();
    Rectangle<float> r = Rectangle<float> (getLocalBounds().toFloat());
        g.setColour (Colours::red);
    g.drawRect(r);
}

void CorrelationMeter::update()
{
    dsp::AudioBlock<float> b = buffer;
    
    for (int i  = 0; i < buffer.getNumChannels(); ++i)
    {
        for(int i = 0; i < buffer.getNumSamples(); i++)
        {
            for (auto j : filters)
                j.process(dsp::ProcessContextReplacing<float>(b));
        }
    }
    
//    auto LT = buffer.getReadPointer(0);
//    auto LTSquared = LT * LT;
//    auto RT = buffer.getReadPointer(1);
//    auto RTSquared = RT * RT;
//
//    auto LTRT = LT * RT;

//    repaint();
}

void CorrelationMeter::drawAverage(Graphics& g,
                 Rectangle<int> bounds,
                 float avg,
                 bool drawBorder)
{
//    Rectangle<float> r;
//    auto b = getLocalBounds();
}
