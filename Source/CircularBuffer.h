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
    using DataType = ... //some container holding T's
    
    void resize(std::size_t s, T fillValue);
    void clear(T fillValue);
    void write(T t);
    DataType& getData();   
    size_t getReadIndex() const;
    size_t getSize() const;
private:
};