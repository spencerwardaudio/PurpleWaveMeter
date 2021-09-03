/*
  ==============================================================================

    CircularBuffer.h
    Created: 29 Jun 2021 4:13:30am
    Author:  Spencer Ward

  ==============================================================================
*/

#pragma once

template<typename T>
struct CircularBuffer
{
    using DataType = std::vector<T>; //some container holding T's
    
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
        //write a value to write pointer index
        elements[writeIndex]->t;
        
        writeIndex++;
        
        if(writeIndex == getSize())
        {
            writeIndex = 0;
        }
    }
    
    DataType& getData()
    {
        return elements;
    }
    
    size_t getReadIndex() const
    {
        readIndex = writeIndex + 1;
        
        if(readIndex == getSize())
        {
            readIndex = 0;
        }
        
        return readIndex;
    }
    
    size_t getSize() const
    {
        return elements.size();
    }
    
private:
    
    std::vector<T> elements { };
    
    size_t writeIndex {};
    size_t readIndex {};
};
