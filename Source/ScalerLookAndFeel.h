/*
  ==============================================================================

    ScalerLookAndFeel.h
    Created: 30 Sep 2022 11:58:30am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GlobalVisualDefines.h"

class ScalerLookAndFeel : public juce::LookAndFeel_V4
{
public:

    void drawRotarySlider (Graphics& g, int x, int y, int width, int height, float sliderPos,
                                            const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override
    {
        auto outline = UI_COLOUR1;
        auto fill    = UI_COLOUR1;

        auto bounds = Rectangle<int> (x, y, width, height).toFloat().reduced (10);

        auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 3.0f;
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

            g.setColour (UI_COLOUR1);
            g.strokePath (valueArc, PathStrokeType (lineW/2, PathStrokeType::curved, PathStrokeType::rounded));
        }

        auto thumbWidth = lineW * 2.0f;
        Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                                 bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));

        g.setColour (MENU_TEXT_COLOUR);
        g.drawLine (bounds.getCentreX(), bounds.getCentreY(), thumbPoint.getX(), thumbPoint.getY(), lineW /3);
        
        g.drawText("50%", bounds, Justification::bottomLeft);
        g.drawText("200%", bounds, Justification::bottomRight);
    }
    
    void drawComboBox (Graphics& g, int width, int height, bool,
                                       int, int, int, int, ComboBox& box) override
    {
        auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
        Rectangle<int> boxBounds (0, 0, width, height);

        g.setColour (box.findColour (ComboBox::backgroundColourId));
        g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

        g.setColour (UI_COLOUR1);
        g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 1.0f);
    }
    
    void getIdealPopupMenuItemSize (const String& text, const bool isSeparator,
                                                    int standardMenuItemHeight, int& idealWidth, int& idealHeight) override
    {
        if (isSeparator)
        {
            idealWidth = 50;
            idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight / 10 : 10;
        }
        else
        {
            auto font = getPopupMenuFont();

            if (standardMenuItemHeight > 0 && font.getHeight() > (float) standardMenuItemHeight / 1.3f)
                font.setHeight ((float) standardMenuItemHeight / 1.3f);

            idealHeight = standardMenuItemHeight > 0 ? standardMenuItemHeight : roundToInt (font.getHeight() * 1.3f);
            idealWidth = font.getStringWidth (text);
        }
    }
    
    void drawPopupMenuItem (Graphics& g, const Rectangle<int>& area,
                            const bool isSeparator, const bool isActive,
                            const bool isHighlighted, const bool isTicked,
                            const bool hasSubMenu, const String& text,
                            const String& shortcutKeyText,
                            const Drawable* icon, const Colour* const textColourToUse)
    {
        {
            if (isSeparator)
            {
                auto r  = area.reduced (5, 0);
                r.removeFromTop (roundToInt (((float) r.getHeight() * 0.5f) - 0.5f));

                g.setColour (findColour (PopupMenu::textColourId).withAlpha (0.3f));
                g.fillRect (r.removeFromTop (1));
            }
            else
            {
                auto textColour = (textColourToUse == nullptr ? findColour (PopupMenu::textColourId)
                                                              : *textColourToUse);

                auto r  = area.reduced (1);

                if (isHighlighted && isActive)
                {
                    g.setColour (findColour (PopupMenu::highlightedBackgroundColourId));
                    g.fillRect (r);

                    g.setColour (findColour (PopupMenu::highlightedTextColourId));
                }
                else
                {
                    g.setColour (textColour.withMultipliedAlpha (isActive ? 1.0f : 0.5f));
                }

                r.reduce (jmin (5, area.getWidth() / 20), 0);

                auto font = getPopupMenuFont();

                auto maxFontHeight = (float) r.getHeight() / 1.3f;

                if (font.getHeight() > maxFontHeight)
                    font.setHeight (maxFontHeight);

                g.setFont (font);

                auto iconArea = r.removeFromLeft (roundToInt (maxFontHeight)).toFloat();

                if (icon != nullptr)
                {
                    icon->drawWithin (g, iconArea, RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.0f);
                    r.removeFromLeft (roundToInt (maxFontHeight * 0.5f));
                }
//                else if (isTicked)
//                {
//                    auto tick = getTickShape (1.0f);
//                    g.fillPath (tick, tick.getTransformToScaleToFit (iconArea.reduced (iconArea.getWidth() / 5, 0).toFloat(), true));
//                }

                if (hasSubMenu)
                {
                    auto arrowH = 0.6f * getPopupMenuFont().getAscent();

                    auto x = static_cast<float> (r.removeFromRight ((int) arrowH).getX());
                    auto halfH = static_cast<float> (r.getCentreY());

                    Path path;
                    path.startNewSubPath (x, halfH - arrowH * 0.5f);
                    path.lineTo (x + arrowH * 0.6f, halfH);
                    path.lineTo (x, halfH + arrowH * 0.5f);

                    g.strokePath (path, PathStrokeType (2.0f));
                }

                r.removeFromRight (3);
                g.drawFittedText (text, r, Justification::centredLeft, 1);

                if (shortcutKeyText.isNotEmpty())
                {
                    auto f2 = font;
                    f2.setHeight (f2.getHeight() * 0.75f);
                    f2.setHorizontalScale (0.95f);
                    g.setFont (f2);

                    g.drawText (shortcutKeyText, r, Justification::centredRight, true);
                }
            }
        }
    }
    
    void positionComboBoxText (ComboBox& box, Label& label) override
    {
        label.setJustificationType(Justification(Justification::horizontallyCentred));
        
        label.setBounds (box.getWidth()/6, 1,
                         box.getWidth() - 15,
                         box.getHeight());
        
        label.setMinimumHorizontalScale(0.1);
        
        label.setFont (getComboBoxFont (box));
    }
};
