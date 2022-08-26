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
struct ReadAllAfterWriteCircularBuffer
{
    using DataType = std::vector<T>;
    
    ReadAllAfterWriteCircularBuffer(T fillValue)
    {
        resize(1, fillValue);
    }
    
    void resize(std::size_t s, T fillValue)
    {
        data.resize(s);
        resetWriteIndex();
    }
    
    void clear(T fillValue)
    {
        //reset data in the buffer to initial Value
        std::fill(data.begin(), data.end(), fillValue);
        resetWriteIndex();
    }
    
    void write(T t)
    {
        auto index = writeIndex.load();
        
        data[index] = t;
        
        index++;
        
        //caching locally so getsize() is only called once
        auto size = getSize();

        if (index > size - 1)
            index = 0;

        writeIndex.store(index);
    }
    
    DataType& getData()
    {
        return data;
    }
    
    size_t getReadIndex() const
    {
        return writeIndex + 2;
    }
    
    size_t getSize() const
    {
        return data.size();
    }
    
private:
    
    void resetWriteIndex()
    {
        writeIndex = 0;
    }
    
    std::atomic<std::size_t> writeIndex {0};
    DataType data;
};

