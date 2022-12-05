/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PurpleWaveMeterAudioProcessor::PurpleWaveMeterAudioProcessor()
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

PurpleWaveMeterAudioProcessor::~PurpleWaveMeterAudioProcessor()
{
//    valueTree.setRootItem (nullptr);
}

//==============================================================================
const String PurpleWaveMeterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PurpleWaveMeterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PurpleWaveMeterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PurpleWaveMeterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PurpleWaveMeterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PurpleWaveMeterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PurpleWaveMeterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PurpleWaveMeterAudioProcessor::setCurrentProgram (int index)
{
}

const String PurpleWaveMeterAudioProcessor::getProgramName (int index)
{
    return {};
}

void PurpleWaveMeterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PurpleWaveMeterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
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

void PurpleWaveMeterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PurpleWaveMeterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PurpleWaveMeterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
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
bool PurpleWaveMeterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PurpleWaveMeterAudioProcessor::createEditor()
{
    return new PurpleWaveMeterAudioProcessorEditor (*this);
}

//==============================================================================
void PurpleWaveMeterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    
    valueTree.writeToStream(mos);
    
    DBG("getStateInformation called");
    
    DBG(valueTree.toXmlString());
}

void PurpleWaveMeterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new PurpleWaveMeterAudioProcessor();
}
