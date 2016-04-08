#pragma once
// TODO: consider renaming this to vmem.h or similar
// most folks categorize vmem as requiring an MMU, so we may want
// a different name

namespace FactUtilEmbedded
{
#define HANDLE_PRECISION uint8_t
#define DESC_PRECISION uint8_t

  namespace vmem
  {
  // 3 layer architecture:
  //
  // layer 1: immobile, predictable file/mem handles
  // layer 2: movable dynamic file/mem descriptors
  // layer 3: file/mem data

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

    class Descriptor
    {
    public:
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

        /*
        struct
        {
          uint16_t type : 2;
          uint16_t allocated : 1;

          // for small CPU mode it's ((0-8191)+1) * 4 = 4 to 32768, multiples of 4
          // for large CPU mode it's ((0-8191)+1) * 16 = 16 to 131072, multiples of 16
          uint16_t size : 13;
        } extended; */
      };
    };

    class Manager
    {
      const void* pool;

      HANDLE_PRECISION handle_count = 0;

      Handle* getHandles() { return nullptr; }
      Descriptor* getDescriptors() { return nullptr; }

    public:
      Manager()
      {

      }


    };
  }
}
