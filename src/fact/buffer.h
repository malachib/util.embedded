#pragma once

#include <stdint.h>
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


    // TODO: consider refactoring to size_t
    template <uint16_t size>
    class MemoryContainer : public MemoryContainerBase<size>
    {
      uint8_t data[size];

    public:
      void clear() { memset(data, 0, size); }
      const uint8_t* getData() const { return data; }
    };

    /*
    template <class T>
    struct ArrayBase
    {
      typedef T*        iterator;
      typedef const T*  const_iterator;
    };*/

    template <class T, uint16_t size>
    class Array : public MemoryContainer<size * sizeof(T)>
      //, public ArrayBase<T>
    {
    public:
      typedef T*        iterator;
      typedef const T*  const_iterator;

      // FIX: definitely will hit data alignment issues here
      T* getData() const { return (T*) MemoryContainer<size * sizeof(T)>::getData(); }
      T& getValue(uint16_t index) const { return getData()[index]; }
      T& operator[](uint16_t index) const { return getValue(index); }

      // NOTE: stl merely uses size(), not getSize()
      uint16_t getSize() const { return size; }

      iterator begin() { return iterator(&getData()[0]); }
      // lifting this from STL example code
      // http://www.aoc.nrao.edu/php/tjuerges/ALMA/STL/html-4.1.2/array-source.html
      iterator end() { return iterator(&getData()[size]); }

      const_iterator begin() const
      { return const_iterator(&getData()[0]); }

      const_iterator end() const
      { return const_iterator(&getData()[size]); }
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

    template <class T, uint16_t size>
    class Array : public MemoryContainer<size * sizeof(T)>
    {
    public:
      typedef T*        iterator;
      typedef const T*  const_iterator;

      Array(T* const data) : MemoryContainer<size * sizeof(T)>(data) {}

      // FIX: definitely will hit data alignment issues here
      T* getData() const { return (T*) MemoryContainerBase::getData(); }
      T& getValue(uint16_t index) const { return getData()[index]; }
      T& operator[](uint16_t index) const { return getValue(index); }
      uint16_t getSize() const { return size; }

      iterator begin() { return iterator(&getData()[0]); }
      // lifting this from STL example code
      // http://www.aoc.nrao.edu/php/tjuerges/ALMA/STL/html-4.1.2/array-source.html
      iterator end() { return iterator(&getData()[size]); }

      const_iterator begin() const
      { return const_iterator(&getData()[0]); }

      const_iterator end() const
      { return const_iterator(&getData()[size]); }
    };
  }

  namespace layer3
  {
    template <class TSize = uint16_t>
    class MemoryContainer : public layer2::MemoryContainerBase
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
      typedef T*        iterator;
      typedef const T*  const_iterator;

      Array(T* const data, TSize size) :
        MemoryContainer<TSize>(data, size * sizeof(T)) {}

      // FIX: definitely will hit data alignment issues here
      T* getData() const { return (T*) MemoryContainer<TSize>::getData(); }
      T& getValue(uint16_t index) const { return getData()[index]; }
      T& operator[](uint16_t index) const { return getValue(index); }
      TSize getSize() const
      { return MemoryContainer<TSize>::getSize() / sizeof(T); }

      iterator begin() { return iterator(&getData()[0]); }
      // lifting this from STL example code
      // http://www.aoc.nrao.edu/php/tjuerges/ALMA/STL/html-4.1.2/array-source.html
      iterator end() { return iterator(&getData()[getSize()]); }

      const_iterator begin() const
      { return const_iterator(&getData()[0]); }

      const_iterator end() const
      { return const_iterator(&getData()[getSize()]); }
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
