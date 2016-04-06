#pragma once

#include "../LinkedList.h"

namespace FactUtilEmbedded
{
  class gc_manager;
  
  class _gc_ptr : DoublyLinkedNode
  {
    friend gc_manager;
    
    void* ptr = nullptr;
    uint16_t size;
    uint8_t refCount = 0;
  
  protected:
    _gc_ptr(void* ptr, uint16_t size) : ptr(ptr), size(size) {}
    
  public:
    void* lock() 
    { 
      refCount++;
      return ptr; 
    }
    
    void unlock()
    {
      refCount--;
    }
  };
  
  template <class T>
  class gc_ptr : public _gc_ptr
  {
    
  };
  
  class gc_manager : public LinkedList<_gc_ptr>
  {
    const uint8_t* buffer;
    const uint16_t size;
    
  public:
    gc_manager(const uint8_t* buffer, const uint16_t size) :
      buffer(buffer), size(size)
    {
      
    }
    
    _gc_ptr alloc(uint16_t size);
  };
}
