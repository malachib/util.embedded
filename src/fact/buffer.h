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
        typedef uint16_t size_t;

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
  namespace experimental {

  struct buffer_traits
  {
    typedef void* ptr_t;
  };

  struct const_buffer_traits
  {
    typedef const void* ptr_t;
  };

  }

    template <class Traits = experimental::buffer_traits>
    class MemoryContainerBase
    {
      void* const data;

    public:
      MemoryContainerBase(void* const data) : data(data) {}

      void* const getData() const { return data; }
    };

    template <uint16_t size, class Traits = experimental::buffer_traits>
    class MemoryContainer :
      public layer1::MemoryContainerBase<size>,
      public layer2::MemoryContainerBase<Traits>
    {
    public:
        typedef uint16_t size_t;

      MemoryContainer(void* const data) : layer2::MemoryContainerBase<Traits>(data) {}

      void clear() { memset(this->getData(), 0, size); }
    };

    template <class T, uint16_t size>
    class Array : public MemoryContainer<size * sizeof(T)>
    {
    public:
      typedef T*        iterator;
      typedef const T*  const_iterator;

      Array(T* const data) : MemoryContainer<size * sizeof(T)>(data) {}

      // FIX: definitely will hit data alignment issues here
      T* getData() const { return (T*) MemoryContainerBase<>::getData(); }
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
    template <class TSize = uint16_t, class Traits = layer2::experimental::buffer_traits>
    class MemoryContainer : public layer2::MemoryContainerBase<Traits>
    {
      const TSize size;

    public:
      MemoryContainer(void* const data, const TSize size) :
        layer2::MemoryContainerBase<Traits>(data), size(size)
      {}

      TSize getSize() const { return size; }
      void clear() { memset(this->getData(), 0, size); }
    };

    template <class T, class TSize = uint16_t, class Traits = layer2::experimental::buffer_traits>
    class Array : public MemoryContainer<TSize, Traits>
    {
        typedef MemoryContainer<TSize, Traits> base_t;
    public:
      typedef T*        iterator;
      typedef const T*  const_iterator;

      Array(T* const data, TSize size) :
        MemoryContainer<TSize>(data, size * sizeof(T)) {}

      // FIX: definitely will hit data alignment issues here
      T* getData() const { return (T*) base_t::getData(); }
      T& getValue(uint16_t index) const { return getData()[index]; }
      T& operator[](uint16_t index) const { return getValue(index); }
      TSize getSize() const
      { return base_t::getSize() / sizeof(T); }

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
      // TODO: consider moving the IMemoryContainer and IArray outside of layer 5
      // since it's so implicit as to be almost explicit that they are a layer 5
      // only scenario
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
