/*
  ==============================================================================

    CircularBuffer.h
    Created: 19 Aug 2022 6:03:35pm
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

template<typename T>
struct CircularBuffer
{
    using vec = std::vector<T>();
    
    void resize(std::size_t s, T fillValue)
    {
        buffer.resize(s, fillValue);
    }
    
    void clear(T fillValue)
    {
        std::fill(buffer.begin(), buffer.end(), fillValue);
    }
    
    void write(T t)
    {
        
        buffer[writeIndex] = t;
        writeIndex++;
        
        if(writeIndex >= buffer.size())
        {
            writeIndex = 0;
        }
    }
    
    vec& getData()
    {
        readIndex = writeIndex + 1;
        return buffer[readIndex];
    }
    
    const size_t getReadIndex()
    {
        readIndex = writeIndex + 1;
        return readIndex;
    }
    
    const size_t getSize()
    {
        return buffer.size();
    }
    
private:
    
    int writeIndex {0 };
    int readIndex {1 };
    std::vector<T> buffer {};
};
