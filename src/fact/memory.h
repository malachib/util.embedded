#pragma once
// TODO: consider renaming this to vmem.h or similar
// most folks categorize vmem as requiring an MMU, so we may want
// a different name

namespace FactUtilEmbedded
{
#define HANDLE_PRECISION uint8_t
#define POOL_PRECISION uint16_t
#define MEMORY_PRECISION uint16_t
//#define DESC_PRECISION uint8_t

  namespace vmem
  {
  // 3 layer architecture:
  //
  // layer 1: immobile, predictable file/mem handles
  // layer 2: movable dynamic file/mem descriptors.  Prepends actual allocated data in pool
  // layer 3: file/mem data

  /*
    // layer 1
    // TDescPrecision is recommended to be either uint8_t or uint16_t
    template <class TDescPrecision>
    class HandleBase
    {
      // while file descriptor we are pointing to
      TDescPrecision descriptor;
    public:

      TDescPrecision getDescriptor() const { return descriptor; }
    };


    class Handle : public HandleBase<DESC_PRECISION>
    {

    };
    */

    class Descriptor;

    class Handle
    {
      Descriptor* descriptor;
    public:
      Descriptor* getDescriptor() { return descriptor; }
      bool hasDescriptor() { return descriptor != nullptr; }
    };

    class Descriptor
    {
    public:
      struct
      {
        uint16_t type : 2;
        uint16_t allocated: 1;

        // for small CPU mode it's ((0-8191)+1) * 4 = 4 to 32768, multiples of 4
        // for large CPU mode it's ((0-8191)+1) * 16 = 16 to 131072, multiples of 16
        uint16_t size : 13;

      } header;

      bool isAllocated() { return header.allocated; }

      bool getSize() { return header.size * 4; }

      /*
      union
      {
        struct
        {
          // types:
          // 0 = standard
          // 1 = standard, linked contiguously to next appearing Descriptor
          // 2 = extended, reserved
          // 3 = extended, reserved
          uint16_t type : 2;

          // whether memory is allocated or free
          uint16_t allocated : 1;

          // describes size of memory block being addressed:
          // for regular small CPU modes it's:
          // 0 = 4,  1 = 8, 2 = 16, 3 = 32, 4 = 64, 5 = 128,
          // 6 = 256, 7 = 512
          // for "large mode" CPU it's:
          // 0 = 4, 1 = 16, 2 = 64, 3 = 256, 4 = 1024, 5 = 4096,
          // 6 = 16384, 7 = 65536
          // FIX: mode 0 is wasted, since we use 16byte blocks for large mode
          uint16_t size_exponent : 3;

          // for small CPU mode it's * 4 (0 to (4096-4))
          // for large CPU mode it's * 16 (0 to (16384-4))
          uint16_t address : 10;

        } primary;

        struct
        {
          uint16_t type : 2;
          uint16_t allocated : 1;

          // for small CPU mode it's ((0-8191)+1) * 4 = 4 to 32768, multiples of 4
          // for large CPU mode it's ((0-8191)+1) * 16 = 16 to 131072, multiples of 16
          uint16_t size : 13;
        } extended;
      }; */
    };

    class Manager
    {
      // TODO: use the buffer/layer code for pool interaction
      const uint8_t* pool;

      const POOL_PRECISION poolSize = 1024;
      HANDLE_PRECISION handle_count = 0;

      POOL_PRECISION getHandleSpaceUsage() const { return handle_count * sizeof(Handle); }
      POOL_PRECISION getPoolTop() const { return poolSize - getHandleSpaceUsage(); }
      // handles are managed going downward like a stack from the top of the memory pool
      Handle* getHandles() const { return (Handle*)(pool + getPoolTop()); }

      // array of descriptors not available when descriptors are free-floating,
      // immediately prepending allocated data
      //Descriptor* getDescriptors() { return nullptr; }

      // do a memory swap of two descriptors.  This has the distinct benefit of
      // hardly using any memory itself
      void swap(Descriptor* a, Descriptor* b)
      {

      }

    public:
      Manager()
      {

      }

      MEMORY_PRECISION memfree() const
      {
        // roll through every handle to tally up free memory
      }

      // attempt a memory defragment
      void defragment()
      {

      }

      // without defragmenting, try to glue existing free blocks together and
      // reduce handle count
      void glueFree()
      {

      }

      // TODO: put this somewhere else as a non-inline function
      Handle& allocate(MEMORY_PRECISION size)
      {
        // to allocate a handle:
        // a) find a handle with a free descriptor 'A' large enough to hold size
        // b) if request size < 'A' size, reduce free size of 'A' and create a
        //    new handle & descriptor contiguously into 'A' memory space.  Then return new handle
        // c) if request size == 'A', merely allocate all of 'A' without creating a new handle
        //    and return existing handle
        // d) if request size > 'A', we may be out of memory; attempt a compact operation *OR* return a NULL handle
        //    depending on policy setting

        Handle* handle = getHandles();

        for(HANDLE_PRECISION i = handle_count; i--; handle++)
        {
          Descriptor* d = handle->getDescriptor();

          if(d != nullptr && !d->isAllocated())
          {
            MEMORY_PRECISION dSize = d->getSize();

            // FIX: only counts if dSize is > size + granularity offset
            if(dSize > size)
            {
              // TODO: split into two
            }
            else if(dSize == size)
            {
              // TODO: repurpose existing one
              return *handle;
            }
          }
        }
      }

      void free(Handle& h)
      {
        h.getDescriptor()->header.allocated = 0;
      }
    };
  }
}
