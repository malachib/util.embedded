#pragma once

#include <Arduino.h>
#include <string.h>

// TODO: utilize some of these templating goodies 
// to auto-detect array size (maybe? with specialization?)
// http://stackoverflow.com/questions/437150/can-someone-explain-this-template-code-that-gives-me-the-size-of-an-array

namespace FactUtilEmbedded
{
  namespace layer1
  {
    template <uint16_t size>
    class MemoryContainerBase
    {
    protected:
    public:  
      static uint16_t getSize() { return size; }
    };
    
    
    template <uint16_t size>
    class MemoryContainer : public MemoryContainerBase<size>
    {
      uint8_t data[size];
      
    public:
      void clear() { memset(data, 0, size); }
      uint8_t* getData() const { return data; }
    };
    
    template <class T, uint16_t size>
    class Array : public MemoryContainer<size * sizeof(T)>
    {
    public:
      // FIX: definitely will hit data alignment issues here
      T* getData() const { return (T*) getData(); }
      T& getValue(uint16_t index) const { return getData()[index]; }
      T& operator[](uint16_t index) const { return getValue(index); }
    };
  }
  
  namespace layer2
  {
    class MemoryContainerBase
    {
      void* const data;
      
    public:
      MemoryContainerBase(void* const data) : data(data) {}
      
      void* getData() const { return data; }
    };
    
    template <uint16_t size>
    class MemoryContainer : 
      public layer1::MemoryContainerBase<size>,
      public layer2::MemoryContainerBase
    {
    public:
      MemoryContainer(void* const data) : layer2::MemoryContainerBase(data) {}
      
      void clear() { memset(getData(), 0, size); }
    };
    
    template <class T, uint16_t size>
    class Array : public MemoryContainer<size * sizeof(T)>
    {
    public:
      Array(T* const data) : MemoryContainer<size * sizeof(T)>(data) {}
      
      // FIX: definitely will hit data alignment issues here
      T* getData() const { return (T*) MemoryContainerBase::getData(); }
      T& getValue(uint16_t index) const { return getData()[index]; }
      T& operator[](uint16_t index) const { return getValue(index); }
    };
  }
  
  namespace layer3
  {
    template <class TSize = uint16_t>
    class MemoryContainer : layer2::MemoryContainerBase
    {
      const TSize size;
      
    public:
      MemoryContainer(void* const data, const TSize size) : 
        layer2::MemoryContainerBase(data), size(size)
      {}
        
      TSize getSize() const { return size; }
      void clear() { memset(getData(), 0, size); }
    };

    template <class T, class TSize = uint16_t>
    class Array : public MemoryContainer<TSize>
    {
    public:
      Array(T* const data, TSize size) : 
        MemoryContainer<TSize>(data, size * sizeof(T)) {}
      
      // FIX: definitely will hit data alignment issues here
      T* getData() const { return (T*) MemoryContainer<TSize>::getData(); }
      T& getValue(uint16_t index) const { return getData()[index]; }
      T& operator[](uint16_t index) const { return getValue(index); }
    };
  }
  
  namespace layer5
  {
    template <class TSize = uint16_t>
    class IMemoryContainer
    {
      virtual TSize getSize() const = 0;
      virtual void clear() = 0;
    };
    
    template <class T, class TSize = uint16_t>
    class IArray
    {
      virtual T& operator[](TSize index) = 0;
    };
    
    template <class TSize = uint16_t>
    class MemoryContainer : 
      public IMemoryContainer<TSize>,
      public layer3::MemoryContainer<TSize>
    {
    public:
      MemoryContainer(void* const data, const TSize size) : 
        layer3::MemoryContainer<TSize>(data, size)
      {}
        
      virtual TSize getSize() const override
       { return layer3::MemoryContainer<TSize>::getSize(); }
      virtual void clear() override { layer3::MemoryContainer<TSize>::clear(); }
    };
  }
}
