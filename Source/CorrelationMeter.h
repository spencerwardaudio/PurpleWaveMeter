/*
  ==============================================================================

    CorrelationMeter.h
    Created: 8 Sep 2022 10:12:40pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "Meter.h"

struct CorrelationMeter : Component
{
    CorrelationMeter(AudioBuffer<float>& buf, double sampleRate);
    void paint(Graphics& g) override;
    void update();
private:
    AudioBuffer<float>& buffer;
    using FilterType = juce::dsp::StateVariableTPTFilter<float>;
    std::array<FilterType, 3> filters;
    
    dsp::ProcessSpec spec;
//    dsp::FilterDesign<FIRCoefficientsPtr> coefficients;
//    dsp::FilterDesign<float> fd;
//    dsp::

    Averager<float> slowAverager{1024*3, 0}, peakAverager{512, 0};
    
    void drawAverage(Graphics& g,
                     Rectangle<int> bounds,
                     float avg,
                     bool drawBorder);
};
