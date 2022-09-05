/*
  ==============================================================================

    Fifo.h
    Created: 1 Sep 2022 12:31:02pm
    Author:  Spencer Ward

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#pragma once

template<typename T>
struct Fifo
{
   void prepare(int numChannels, int numSamples)
   {
       //initialize all Fifo buffers with Max size of sample block

       for( auto& buf : buffer )
       {
           buf.setSize(numChannels, numSamples);
           buf.clear();
       }

   }
   
   bool push( const T& t)
   {
       auto write = fifo.write(1);
       if( write.blockSize1 >= 1 )
       {
           buffer[write.startIndex1] = t;
           return true;
       }
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
       return false;
   }
   
private:
   
   static const int Capacity = 2;
   std::array<T, Capacity> buffer;
   AbstractFifo fifo{ Capacity };
};

//==============================================================================
