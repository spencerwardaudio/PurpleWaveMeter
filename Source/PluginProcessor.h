/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

template<typename T>
struct Fifo
{
    void prepare(int numSamples)
    {
        //initialize all Fifo buffers with Max size of sample block
        DBG("numSamples: " << numSamples);
        
        //buffer for left channel
        buffer[0].setSize(1, numSamples);
        
        //buffer for right channel
        buffer[1].setSize(1, numSamples);
        
        buffer[0].clear();
        buffer[1].clear();

        prepared = true;
    }
    
    bool push( const T& t)
    {
        auto write = fifo.write(1);
        if( write.blockSize1 >= 1 )
        {
            buffer[write.startIndex1] = t;
            return true;
        }
        DBG("push");
        return false;
    }
    
    bool pull( T& t)
    {
        auto read = fifo.read(1);
        if( read.blockSize1 >= 1 )
        {
            t = buffer[read.startIndex1];
            return true;
        }
        DBG("pull");
        return false;
    }
    
    
private:
    
    bool prepared = false;
    static constexpr int Capacity = 2;
    std::array<AudioBuffer<float>, Capacity> buffer;
    AbstractFifo fifo{ Capacity };
};

//==============================================================================


class Pfmcpp_project10AudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    Pfmcpp_project10AudioProcessor();
    ~Pfmcpp_project10AudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    Fifo<AudioBuffer<float>> fifo;
    
private:
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessor)
};
