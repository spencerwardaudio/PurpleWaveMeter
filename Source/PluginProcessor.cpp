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
                       )
#endif
{
    
    valueTree.setProperty (decayRate, 1, nullptr);
    
    valueTree.setProperty (averageTime, 1, nullptr);
    
    valueTree.setProperty (meterView, 1, nullptr);
    
    valueTree.setProperty (goniometerScale, 0.0, nullptr);
    
    valueTree.setProperty (enableHold, 0, nullptr);
    
    valueTree.setProperty (holdTime, 0, nullptr);
    
    valueTree.setProperty (histogramView, 1, nullptr);
    
    valueTree.setProperty (peakThreshold, 0.0, nullptr);
    
    valueTree.setProperty (rmsThreshold, 0.0, nullptr);
    
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
    juce::MemoryOutputStream mos(destData, true);
    
    valueTree.writeToStream(mos);
    
    DBG("getStateInformation called");
    
    DBG(valueTree.toXmlString());
}

void Pfmcpp_project10AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = ValueTree::readFromData(data, static_cast<size_t>(sizeInBytes));

    if(tree.isValid())
    {
        valueTree = tree;
        DBG("setStateInformation called");
    }
    
    DBG(valueTree.toXmlString());
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Pfmcpp_project10AudioProcessor();
}
