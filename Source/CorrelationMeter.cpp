/*
  ==============================================================================

    CorrelationMeter.cpp
    Created: 8 Sep 2022 10:12:40pm
    Author:  Spencer Ward

  ==============================================================================
*/

#include "CorrelationMeter.h"

CorrelationMeter::CorrelationMeter(AudioBuffer<float>& buf, double sampleRate) : buffer(buf), lowpassFilter(dsp::FilterDesign<float>::designFIRLowpassWindowMethod(100, sampleRate, 2, juce::dsp::WindowingFunction<float>::blackmanHarris))
{
    spec.sampleRate = sampleRate;
    spec.numChannels = 2;
    spec.maximumBlockSize = buffer.getNumSamples();
    
    lowpassFilter.prepare(spec);
    lowpassFilter.reset();
}

void CorrelationMeter::paint(Graphics& g)
{
    //    drawAverage();
    Rectangle<float> r = Rectangle<float> (getLocalBounds().toFloat());
        g.setColour (Colours::red);
    g.drawRect(r);
}

void CorrelationMeter::update()
{
    dsp::AudioBlock<float> b = buffer;
    
    lowpassFilter.process(dsp::ProcessContextReplacing<float>(b));

    auto LT = buffer.getReadPointer(0);
    auto LTSquared = LT * LT;
    auto RT = buffer.getReadPointer(1);
    float RTSquared = RT * RT;

    auto LTRT = LT * RT;

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
