#pragma once

#include <Arduino.h>
#include <string.h>

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
      uint8_t* const getData() const { return data; }
    };
  }
  
  namespace layer2
  {
    class MemoryContainerBase
    {
      void* const data;
      
    public:
      MemoryContainerBase(void* const data) : data(data) {}
      
      void* const getData() const { return data; }
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
  }
  
  namespace layer5
  {
    template <class TSize = uint16_t>
    class IMemoryContainer
    {
      virtual TSize getSize() const = 0;
      virtual void clear() = 0;
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
