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
  template <class TArray, class T>
  class CircularBuffer : public CircularBufferBase<T>
  {
    TArray array;

    //T* buffer;
    // position of where next put will occur
    uint16_t position = 0;
    // position of where next get will occur
    //uint16_t position_get = 0;
    //uint16_t size;

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
      auto size = array.getSize();
      auto position_get = position >= currentCapacity ?
        (position - currentCapacity) :
        ((size) - (currentCapacity - position));

      return position_get;
    }

protected:
    // for derived classes which don't need to initialize underlying array
    CircularBuffer() {}

  public:
    CircularBuffer(const TArray& array) :
      array(array) //,
      //size(size)
    {
      //buffer = bufferToUse;
    }

    //CircularBuffer(uint16_t size) : size(size) {}

    //CircularBuffer() {}

    /*
    void setBuffer(T* buffer, uint16_t size)
    {
      this->buffer = buffer;
      this->size = size;
    }*/

    // normally we would scope this as protected, but
    // for speed we want to be able to peekLast / incrementPosition
    // instead of copying large messages around with a regular 'put'
    void incrementPosition()
    {
      auto size = array.getSize();
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
      auto buffer = array.getData();
      return buffer[getPositionGet()];
    }

    // Returns empty, freshly available buffer slot for a put
    // mainly useful for fast-writing to circular buffer manually vs
    // a full put operation
    const T& peekLast() const
    {
      auto buffer = array.getData();
      return buffer[position];
    }

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
      auto buffer = array.getData();
      buffer[position] = value;
      incrementPosition();
    }

    // size override for those times one doesn't want to copy
    // the entire object, for esoteric optimization reasons.  Should
    // always optimize to be a constant, so no overhead
    void put(T* value, uint16_t sizeOverride = 0)
    {
      auto buffer = array.getData();
      memcpy(&buffer[position], value, sizeOverride ? sizeOverride : sizeof(T));
      incrementPosition();
    }

    void reset()
    {
      position = 0;
      currentCapacity = 0;
      //position_get = 0;
    }

      const TArray& getUnderlyingArray() const { return array; }

    void debugPrint() const
    {
#if defined(ARDUINO) && defined(DEBUG)
      auto buffer = array.getData();
      auto size = array.getSize();

      for(int i = 0; i < size; i++)
      {
        Serial << buffer[i] << F(", ");
      }
  #endif
    }
  };

namespace layer1
{
    template <class T, uint16_t size>
    class CircularBuffer : public FactUtilEmbedded::CircularBuffer<layer1::Array<T, size>, T>
    {
    public:
        // layer1 version array is pre-allocated.  Be careful using
        // this constructor though, because this doesn't explicitly
        // set the values to 0 (presumably you'll be using a static one which
        // starts as 0 already)
        CircularBuffer() {}
    };
}

namespace layer2
{
    template <class T, uint16_t size>
    class CircularBuffer : public FactUtilEmbedded::CircularBuffer<layer2::Array<T, size>, T>
    {
        typedef layer2::Array<T, size> array_t;

    public:
        // layer1 version array is pre-allocated.  Be careful using
        // this constructor though, because this doesn't explicitly
        // set the values to 0 (presumably you'll be using a static one which
        // starts as 0 already)
        CircularBuffer(T* bufferToUse) :
          FactUtilEmbedded::CircularBuffer<array_t, T>(array_t(bufferToUse))
              {}
    };
}

namespace layer3
{
  template <class T>
  class CircularBuffer : public FactUtilEmbedded::CircularBuffer<layer3::Array<T, uint16_t>, T>
  {
      typedef layer3::Array<T, uint16_t> array_t;

  public:
      CircularBuffer(T* bufferToUse, uint16_t size) :
        FactUtilEmbedded::CircularBuffer<array_t, T>(array_t(bufferToUse, size))
            {}

  };
}

namespace layer5
{
    template <class T>
    class ICircularBuffer
    {
        virtual void put(const T& value) = 0;
        virtual const T& peek() const = 0;
        virtual const T& get() = 0;
    };

  template <class T>
  class CircularBuffer :
    public FactUtilEmbedded::CircularBuffer<layer5::IArray<T, uint16_t>&, T>,
    public ICircularBuffer<T>
  {
      typedef layer5::IArray<T, uint16_t>& array_t;

  public:
      /*
      CircularBuffer(T* bufferToUse, uint16_t size) :
        FactUtilEmbedded::CircularBuffer<array_t, T>(array_t(bufferToUse, size))
            {} */
        CircularBuffer(array_t array) :
        FactUtilEmbedded::CircularBuffer<array_t, T>(array)
        {}
  };
}
}
