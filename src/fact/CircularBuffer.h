#pragma once

#include "buffer.h"

namespace FactUtilEmbedded
{

  template <class T>
  class CircularBufferBase
  {
  protected:
    //T* buffer;  // still mysterious why I can't use this
  };

  // TODO: Do pointer arithmatic with this one.  Will take more memory but goes faster
  template <class T>
  class CircularBufferFast : public CircularBufferBase<T>
  {
  };

  // TODO: make position/size etc templated also to support a byte-version of this
  template <class T>
  class CircularBuffer : public CircularBufferBase<T>
  {
    //layer3::Array<T, uint16_t> array;

    T* buffer;
    // position of where next put will occur
    uint16_t position = 0;
    // position of where next get will occur
    //uint16_t position_get = 0;
    uint16_t size;

    uint16_t currentCapacity = 0;

    /*
    void incrementPositionGet()
    {
      if(++position_get >= size)
        position_get = 0;
    }*/

  #ifdef UNIT_TEST
  public:
  #endif
    uint16_t getPosition() const { return position; }
    uint16_t getPositionGet() const
    {
      auto position_get = position >= currentCapacity ?
        (position - currentCapacity) :
        ((size) - (currentCapacity - position));

      return position_get;
    }

  public:
    CircularBuffer<T>(T* bufferToUse, uint16_t size) :
      //array(bufferToUse, size),
      size(size)
    {
      buffer = bufferToUse;
    }

    //CircularBuffer(uint16_t size) : size(size) {}

    //CircularBuffer() {}

    void setBuffer(T* buffer, uint16_t size)
    {
      this->buffer = buffer;
      this->size = size;
    }

    // normally we would scope this as protected, but
    // for speed we want to be able to peekLast / incrementPosition
    // instead of copying large messages around with a regular 'put'
    void incrementPosition()
    {
      //if(available() == size)
        //incrementPositionGet();

      // TODO: do a strict-mode branch where we don't check
      if(currentCapacity < size)
        currentCapacity++;

      if(++position >= size)
        position = 0;
    }



    //T get();
    // TODO: See if template classes really *always* generate inline functions
    // TODO: turn this into a byte oriented version, then templatize the child class so that
    // we aren't generating inline functions all over town
    // TODO: Try to optimize this with a return of const T&
    const T& get()
    {
      const T& value = peek();
      //incrementPositionGet();
      currentCapacity--;
      return value;
    }

    const T& peek() const
    {
      return buffer[getPositionGet()];
    }

    // Returns empty, freshly available buffer slot for a put
    // mainly useful for fast-writing to circular buffer manually vs
    // a full put operation
    const T& peekLast() const { return buffer[position]; }

    // acquire how many elements are available to be read
    uint16_t available() const
    {
      return currentCapacity;
      /*
      if(position_get <= position)
      {
        return position - position_get;
      }
      else
      {
        return ((size) - position_get) + position;
      }*/
    }

    void put(const T& value)
    {
      buffer[position] = value;
      incrementPosition();
    }

    // size override for those times one doesn't want to copy
    // the entire object, for esoteric optimization reasons.  Should
    // always optimize to be a constant, so no overhead
    void put(T* value, uint16_t sizeOverride = 0)
    {
      memcpy(&buffer[position], value, sizeOverride ? sizeOverride : sizeof(T));
      incrementPosition();
    }

    void reset()
    {
      position = 0;
      currentCapacity = 0;
      //position_get = 0;
    }

    void debugPrint() const
    {
  #if defined(ARDUINO) && defined(DEBUG)
      for(int i = 0; i < size; i++)
      {
        Serial << buffer[i] << F(", ");
      }
  #endif
    }
  };
}
