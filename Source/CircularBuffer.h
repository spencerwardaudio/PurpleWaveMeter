/*
  ==============================================================================

    CircularBuffer.h
    Created: 29 Jun 2021 4:13:30am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

template<typename T>
struct CircularBuffer
{
    using DataType = std::vector<T>;
    
    void resize(std::size_t s, T fillValue)
    {
        elements.resize(s);
        
        clear(fillValue);
    }
    
    void clear(T fillValue)
    {
        //reset elements in the buffer to initial Value
        std::fill(elements.begin(), elements.end(), fillValue);
    }
    
    void write(T t)
    {
        writeIndex = readIndex++;
        //write a value to write pointer index
        elements[writeIndex] = t;

        if(readIndex >= getSize())
        {
            readIndex = 0;
        }
        
        DBG("readIndex: " << readIndex);
        DBG("writeIndex: "<< writeIndex);
    }
    
    DataType& getData()
    {
        return elements;
    }
    
    const size_t getReadIndex()
    {
        return readIndex;
    }
    
    const size_t getSize()
    {
        return elements.size();
    }
    
private:
    
    std::vector<T> elements { };
    
    int writeIndex {0};
    int readIndex {0};
};

