/*
  ==============================================================================

    ScalerLookAndFeel.h
    Created: 30 Sep 2022 11:58:30am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class ScalerLookAndFeel : public juce::LookAndFeel_V4
{
public:

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                            const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        //TODO draw ellipse for the knob
        
        //TODO draw rectangle for the knob arrow and make it arc
        //TODO add two labels to the slider
        auto outline = Colours::indianred;
        auto fill    = Colours::indianred;

        auto bounds = Rectangle<int> (x, y, width, height).toFloat().reduced (10);

        auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = jmin (8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        Path backgroundArc;
        backgroundArc.addCentredArc (bounds.getCentreX(),
                                     bounds.getCentreY(),
                                     arcRadius,
                                     arcRadius,
                                     0.0f,
                                     rotaryStartAngle,
                                     rotaryEndAngle,
                                     true);

        g.setColour (fill);
        g.strokePath (backgroundArc, PathStrokeType (lineW /3, PathStrokeType::curved, PathStrokeType::rounded));

        if (slider.isEnabled())
        {
            Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(),
                                    bounds.getCentreY(),
                                    arcRadius,
                                    arcRadius,
                                    0.0f,
                                    rotaryStartAngle,
                                    toAngle,
                                    true);

            g.setColour (Colours::indianred);
            g.strokePath (valueArc, PathStrokeType (lineW/2, PathStrokeType::curved, PathStrokeType::rounded));
        }

        auto thumbWidth = lineW * 2.0f;
        Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                                 bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));

        g.setColour (Colours::indianred);
        g.drawLine (bounds.getCentreX(), bounds.getCentreY(), thumbPoint.getX(), thumbPoint.getY(), lineW /3);
    }
    
};
