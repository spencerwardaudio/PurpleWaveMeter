/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <array>

struct VariableSizedBuffer
{
    void prepare(int size)
    {
        buffer.setSize(1, size);
        buffer.clear();
        prepared = true;
    }
    
    void clone(const AudioBuffer<float>& other);
    AudioBuffer<float>& getBuffer() { return buffer; }
    size_t getNumSamples() const { return numSamples; }
private:
    AudioBuffer<float> buffer;
    size_t numSamples { 0 };
    bool prepared = false;
    
    template<typename BufferType>
    void clear(const BufferType& other)
    {
        jassert(prepared);
        jassert(other.getNumSamples() <= buffer.getNumSamples() );
        buffer.clear();
    }
};

template<typename T>
struct Fifo
{
    void prepare(int numSamples)
    {
        //initialize all Fifo buffers with Max size of sample block
        DBG("numSamples: " << numSamples);
        for(auto& buffer : buffers )
            buffer.prepare(numSamples);
    }
    
    bool push( const T& t)
    {
        auto write = fifo.write(1);
        if( write.blockSize1 >= 1 )
        {
            auto& buffer = buffers[write.startIndex1];
            buffer.clone(t);
            return true;
        }
        return false;
    }
    
    bool pull( T& t)
    {
        auto read = fifo.read(1);
        if( read.blockSize1 >= 1 )
        {
            auto& buffer = buffers[read.startIndex1];
            t.clone(buffer);
            return true;
        }
        return false;
    }
    
private:
    static constexpr int Capacity = 5;
    std::array<VariableSizedBuffer, Capacity> buffers;
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
    
    Fifo<float> fifo;
    
private:
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pfmcpp_project10AudioProcessor)
};
