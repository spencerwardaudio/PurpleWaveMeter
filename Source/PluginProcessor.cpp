/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Pfmcpp_project10AudioProcessor::Pfmcpp_project10AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), valueTree("parameter")
#endif
{
    static juce::Identifier parameter ("Params"); // pre-create an Identifier
    juce::ValueTree valueTree (parameter);           // This is a valid node, of type "MyNode"
    
    static juce::Identifier decayRate ("decayRate");
    valueTree.setProperty (decayRate, "-3dB/s", &undoManager);
    DBG(valueTree.toXmlString());
    
    static juce::Identifier averageTime ("averageTime");
    valueTree.setProperty (averageTime, "100ms", &undoManager);
    DBG(valueTree.toXmlString());
    
    static juce::Identifier meterView ("meterView");
    valueTree.setProperty (meterView, "Both", &undoManager);
    DBG(valueTree.toXmlString());
    

    
}

Pfmcpp_project10AudioProcessor::~Pfmcpp_project10AudioProcessor()
{
//    valueTree.setRootItem (nullptr);
}

//==============================================================================
const String Pfmcpp_project10AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Pfmcpp_project10AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Pfmcpp_project10AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Pfmcpp_project10AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Pfmcpp_project10AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Pfmcpp_project10AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Pfmcpp_project10AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Pfmcpp_project10AudioProcessor::setCurrentProgram (int index)
{
}

const String Pfmcpp_project10AudioProcessor::getProgramName (int index)
{
    return {};
}

void Pfmcpp_project10AudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void Pfmcpp_project10AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fifo.prepare(getNumOutputChannels(), samplesPerBlock);
    
    maxBufferSize = samplesPerBlock;
    
#if VerifyDbScale
    
    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
    oscl.initialise ([] (float x) { return std::sin (x); }, 128); 
    oscl.setFrequency(440.f);
    oscl.prepare(spec);

#endif
    
}

void Pfmcpp_project10AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Pfmcpp_project10AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void Pfmcpp_project10AudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
 
#if VerifyDbScale
    
    auto gainLvl = Decibels::decibelsToGain(-6.f);

    for(int i = 0; i < buffer.getNumSamples(); ++i)
    {
        auto sample = oscl.processSample(0) * gainLvl;
        
        for(int channel = 0; channel < buffer.getNumChannels(); ++channel)
        {
            buffer.setSample(channel, i, sample);
        }
    }

#endif
    
    fifo.push(buffer);

}

//==============================================================================
bool Pfmcpp_project10AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* Pfmcpp_project10AudioProcessor::createEditor()
{
    return new Pfmcpp_project10AudioProcessorEditor (*this);
}

//==============================================================================
void Pfmcpp_project10AudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
//    juce::String name (valueTree.getProperty (propertyName));

//    valueTree.getParent().
//    std::unique_ptr<juce::XmlElement> xml (valueTree.createXml());
//    copyXmlToBinary (*xml, destData);
    std::unique_ptr<XmlElement> xml(valueTree.createXml());
    copyXmlToBinary(*xml, destData);

    int numProperties = valueTree.getNumProperties();
    DBG(numProperties);
    DBG(valueTree.toXmlString() << " getting values out");
    
}

void Pfmcpp_project10AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
//    // whose contents will have been created by the getStateInformation() call.

//    if( valueTree.isValid() && hasNeedProperties(valueTree))
//    {
//        state = valueTree;
//    }
//    valueTree.setProperty("decayControl", label.getText(), &undoMan);
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary (data, sizeInBytes));

    if (xml.get() != nullptr)
        if (xml->hasTagName (valueTree.getType()))
            valueTree.copyPropertiesFrom((juce::ValueTree::fromXml (*xml)), &undoManager);
    
    static juce::Identifier propertyName ("name");
    valueTree.setProperty (propertyName, "Fluffmuff", nullptr);
    DBG(valueTree.toXmlString());
    
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pfmcpp_project10AudioProcessor();
}
