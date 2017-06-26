#pragma once

#include "../fact/buffer.h"
#include "../LinkedList.h"

#include "memory.h"

#define GC_DEBUG

// The point of the GC experiment is not automatic freeing,
// but rather gaurunteed fragmentation avoidance

namespace FactUtilEmbedded { namespace experimental {

class GC_base;

extern GC_base& _gc;


class GCObject : public SinglyLinkedNode
{
public:
    // NOTE: probably is a performance penalty, not aligning
    // to word boundaries.  If we like the GC overall,
    // revisit this
    typedef uint8_t* ptr_t;

    // TODO: Put this behavior into underlying SinglyLinkedNode
    GCObject() { next = nullptr; }

    ptr_t data;
    struct
    {
        uint32_t size : 16;
        uint32_t pinned : 1;
        uint32_t busy : 1; // means we can't pin this (likely being moved)
        // means we can never pin this, and instead have to use read/write commands on it
        // NOT implemented yet
        uint32_t fragmentable : 1;
        uint32_t handler : 3; // which operation manager are we using (0 for none/no special)

    } attr;

    size_t size() { return attr.size; }
    uint8_t handler() { return attr.handler; }

    // Shrinks free_gco by a specified amount, moving pointer upwards
    void shrink_up(size_t size)
    {
        data += size;
        attr.size -= size;
    }


    // Shrinks free_gco by a specified amount, leaving pointer intact
    void shrink_down(size_t size)
    {
        attr.size -= size;
    }

    void lock() { attr.pinned = true; }
    void unlock() { attr.pinned = false; }


#ifdef UNUSEDXXX
    /**
     * @brief is_free - see if slot used by this GCObject is available *and* no other GCObjects follow it
     * @return
     */
    bool is_free() { return data != nullptr; }

    /**
     * @brief is_reusable - see if slot used by this GCObject is available but other GCObjects may follow it
     * @return
     */
    bool is_reusable() { return size == 0; }
#endif
};


// For specialized GCObjects.  Here we can extend core funcionality for things like:
// a) virtual memory behavior
// b) GCHandleManager GCObject->next reordering
class GCOperationManager
{
public:
    virtual void lock(GCObject& gco) = 0;
    virtual void move(GCObject& gco) = 0;
    virtual void housekeep(GCObject& gco) = 0;
};



class GC_base
{

#ifdef UNIT_TEST
public:
#endif

#ifdef UNUSEDXXX
    size_t used() const
    {
        auto gcobjects = static_cast<GCObject*>(buffer.getData());

        while(gcobjects->is_free())
        {
            gcobjects++;
        }
    }
#endif


// This does fancy in-place free node resizing rather than deallocating and reallocating
// the free node.  Faster but results in somewhat confusing allocation pointers
#define __ALLOC2

    uint8_t* __alloc(size_t& len)
    {
        //auto node = free.getHead();
        ll_free_iterator_t i(free);

        // TODO: overload i bool operator so that
        // we can properly do while(i)
        while(i())
        {
            free_gco_t* node = i;

            // If we can fit this allocation into the inspected
            // node
            if(node->size() > len)
            {
                uint8_t* location = node->data;

#ifndef __ALLOC2
                // then remove the node from the free memory
                // list
                free.remove(node);
#endif
                // If free memory chunk size - request len size
                // < size of new free_gco_t to be created,
                // then we expand our request length slightly
                // and burn up that memory.  Otherwise,
                // we use leftover memory and create a new
                // free_gco_t in there
                if((node->size() - len) < sizeof(free_gco_t))
                {
                    len = node->size();

#ifdef __ALLOC2
                    free.remove(node);
#endif
                }
                else
                {
#ifdef __ALLOC2
                    // this funky technique means our data ptrs are near the end of
                    // the free available memory chunk instead of the beginning,
                    // but lets us leave the free node ptr in place (since the free node
                    // data lives IN the same memory chunk to which it points)
                    location += node->size() - len;
                    node->shrink_down(len);
#else
                    // re-add the node with adjusted parameters
                    addfree(node->data + len, node->size - len);
#endif
                }

                // return the previous free node data ptr
                return location;
            }
            else if(node->size() == len)
            {

            }

            i++;
        }

        return nullptr;
    }

    size_t _allocated() const
    {
        layer1::LinkedListIterator<GCObject> i(allocated);

        size_t total = 0;

        while(i())
        {
            total += i.getCurrent()->size();
            i++;
        }

        return total;
    }


    void defragment()
    {

    }


protected:
#ifdef UNIT_TEST
public:
#endif

    typedef GCObject free_gco_t;
    typedef GCObject allocated_gco_t;
    typedef layer1::LinkedListIterator<allocated_gco_t> ll_iterator_t;
    typedef layer1::LinkedListIterator<free_gco_t> ll_free_iterator_t;

    SinglyLinkedList free;
    SinglyLinkedList allocated;

#ifdef GC_DEBUG
    void walk_free(const char* msg)
    {
        ll_free_iterator_t i(free);

        printf("%s\r\n", msg);

        while(i())
        {
            free_gco_t* current = i;

            printf("Node: addr = %p, size = %lu, next = %p\r\n", current->data, current->size(), current->getNext());
            //std::clog << "TEST";

            i++;
        }
    }
#else
    void walk_free(const char* msg) {}
#endif

    /**
     * @brief recombine - take two contiguous free_gco_t and combine them
     * @param gco_primary
     * @param gco_secondary
     */
    inline void recombine(free_gco_t* gco_primary, free_gco_t* gco_secondary)
    {
#ifdef GC_DEBUG
        if(gco_secondary != gco_primary->getNext())
            printf("recombine failure: expected contiguous linked nodes\r\n");

        if(gco_primary->data + gco_primary->size() != gco_secondary->data)
            printf("recombine failure: incorrect sizes\r\n");
#endif

        gco_primary->attr.size += gco_secondary->size();
        // TODO: make a free.removeNext() call for compatibility with
        // lists with tails
        gco_primary->removeNext();

    }

    void recombine_all()
    {
        ll_iterator_t i(free);

        while(i())
        {
            free_gco_t* current = i;
            auto next = static_cast<free_gco_t*>(current->getNext());

            if(current->data + current->size() == next->data)
            {
                recombine(current, next);
                walk_free("recombined");
            }

            i++;
        }
    }


    /**
     * Moves allocated region backwards into a preceding free region
     * and moves the free region forward
     * @param a
     * @param free
     * @param free_prior required so that 'next' pointer is updated properly
     */
    void swap(allocated_gco_t* a, free_gco_t* free, free_gco_t* free_prior)
    {
        // TODO: assert free->prior->next == free


    }

    // Moves an allocated chunk to the specified free block
    void move(allocated_gco_t* allocated, free_gco_t* free, free_gco_t* free_prior)
    {
        // TODO: assert free->size is big enough
        // TODO: assert free->prior->next == free
    }


    /**
     * @brief addfree_sorted - stuff free_gco back into free-linkedlist, sorted by data ptr
     * @param free_gco
     */
    void addfree_sorted(free_gco_t* free_gco)
    {
        layer1::LinkedListIterator<GCObject> i(free);
        uint8_t* location = free_gco->data;

        // loop through all free nodes
        while(i())
        {
            free_gco_t* current = i;

            // FIX: move i.getNext() to be i.moveNext();
            //free_gco_t* next = static_cast<free_gco_t*>(current->getNext());

            // If location being freed is < current inspected location
            if(location < current->data)
            {
                // FIX: Problem, nodes appear to be sorting backwards

                // insert before inspected current location
                // if next location ptr is above this one
                free.insert(i, free_gco);
                return;
            }

            /*
            // if we're at the end of the list
            if(!next)
            {
                // do addAfter, equivelant but faster to add

                //i.getCurrent()->insertBetween(i, nullptr);
                // TODO: add in an insertAfter/addAfter into SinglyLinkedList
                //free.insertBetween(i, nullptr, free_gco);

                // TODO: above stuff doesn't quite add to end properly, so use this slow one:
                free.add(free_gco);
                return;
            } */

            i++;
        }

        // if there were no iterations at all...
        //free.insertAtBeginning(free_gco);
        free.add(free_gco);
    }

    /**
     * @brief addfree - register a free location
     * @param location
     * @param len
     */
    void addfree(const uint8_t* location, size_t len)
    {
        // TODO: assert len > sizeof(free_gco_t)

        // FIX: We're burning up space in free memory by tracking
        // this 'self' pointer.  Not needed, but using anyway
        // *might* be useful if we decide to
        // store free GCObjects elsewhere - though unlikely we'll do that
        //GCObject* free_gco = static_cast<GCObject*>((void*)location);
        free_gco_t* free_gco = new ((void*)location) free_gco_t();

        free_gco->data = (uint8_t*)location;
        free_gco->attr.size = len;

        addfree_sorted(free_gco);

#ifdef GC_DEBUG
        printf("Freeing memory location: %p\r\n", location);
#endif

        walk_free("add_free");

        if(free_gco->getNext())
        {
            GCObject* next = static_cast<GCObject*>(free_gco->getNext());

#ifdef GC_DEBUG
            if(location != free_gco->data)
                printf("addfree failure: location ptr mismatch");
#endif
            if(location + free_gco->size() == next->data)
            {
                recombine(free_gco, next);
                walk_free("recombined");
            }
//#ifdef DEBUG
            else if(location + free_gco->size() > next->data)
            {
                // ERROR: free memory spots should not overlap

            }
//#endif
        }
    }

    void _free(GCObject& gco)
    {
        addfree(gco.data, gco.size());
        allocated.remove(&gco);
    }


    uint8_t* lock(GCObject& gco)
    {
        gco.attr.pinned = true;
        return gco.data;
    }


    void unlock(GCObject& gco)
    {
        gco.attr.pinned = false;
    }


    size_t available() const
    {
        layer1::LinkedListIterator<GCObject> i(free);

        size_t total = 0;

        while(i())
        {
            total += i.getCurrent()->size();
            i++;
        }

        return total;
    }


    void alloc(GCObject* gco, size_t len)
    {
        gco->data = __alloc(len);

        if(gco->data)
        {
#ifdef GC_DEBUG
            printf("alloc: buffer = %p / size = %lu\r\n", gco->data, len);
#endif
            allocated.insertAtBeginning(gco);
            gco->attr.size = len;
        }
    }

    GCObject alloc(size_t len)
    {
        GCObject gco;
        alloc(&gco, len);
        return gco;
    }

    /*
     * FIX: document whether we need to lock down gco before we can grow it and
     * conversly whether being locked down is allowed
     */
    void grow(GCObject& gco, size_t len_to_grow)
    {
        // TODO: see if we can EZ grow the thing and if not,
        // a) see if we can move it somewhere to grow it there
        // b) see if we can compact things down
    }


    void shrink(GCObject& gco, size_t len_to_shrink)
    {
        // shrinking is always easier than growing
    }


    // these fragment functions are special because you don't have to issue lock()
    // before using them.  Right now fragment feature is not implemented, so
    // this is just a pass thru
    size_t read_fragment(GCObject& gco, size_t pos, uint8_t* buffer, size_t length)
    {
        uint8_t* in_buffer = lock(gco);
        memcpy(buffer, in_buffer + pos, length);
        unlock(gco);
        return length;
    }


    size_t write_fragment(GCObject& gco, size_t pos, const uint8_t* buffer, size_t length)
    {
        uint8_t* out_buffer = lock(gco);
        memcpy(out_buffer + pos, buffer, length);
        unlock(gco);
        return length;
    }
};


template <class T> //, GC_base& gc = _gc>
class GCPointer
{
    GCObject* gco;

public:
    GCPointer(GCObject* gco) : gco(gco)
    {
    }

    const T* lock() { return _gc.lock(*gco); }
    void unlock()   { _gc.unlock(*gco); }
    operator const T* () { return _gc.lock(*gco); }
};

template <class T>
class GCPointer2 : public GCPointer<T>
{
    GCObject _gco;
public:
    GCPointer2() : GCPointer<T>(&_gco) {}
    GCPointer2(const GCPointer2&& moveFrom) : GCPointer<T>(&_gco)
    {
        _gco.data = moveFrom._gco.data;
        _gco.size = moveFrom._gco.size;
    }
};

//template <size_t size>
//class GC;

template <class T>
class GCPointer3
{
    uint16_t refCount;

public:
    GC_base& gc;
    GCObject gco;

    GCPointer3(GC_base& gc) : gc(gc)
    {
        //refCount = 0;
    }

    GCPointer3(GC_base& gc, GCObject& gco) : gc(gc), gco(gco)
    {
        //refCount = 0;
    }

    // Can only be moved, never copied
    // Not working quite though... commenting out destructor doesnt help
    GCPointer3(GCPointer3&& gcp) : gc(gcp.gc) //, refCount(gcp.refCount)
    {
        // FIX: A move means the gco address is changing,
        // which means we need to adjust the preceding gco's
        // next pointer
        //gco = gcp.gco;
        gco.attr.size = -1;
        //printf("!!!! GOT HERE !!!!");
    }


    /*
    GCPointer3(const GCPointer3& gcp) : gc(gcp.gc)
    {

    } */

    T* lock()
    {
        refCount++;
        return (T*)gc.lock(gco);
    }
    void unlock()
    {
        refCount--;
        gc.unlock(gco);
    }

    operator T* ()  { return lock(); }

    ~GCPointer3()
    {
        unlock();
    }
};


template <size_t size, size_t max_handlers = 0>
class GC : public GC_base
{
#ifdef UNIT_TEST
public:
#endif
    layer1::MemoryContainer<size> buffer;
    layer1::Array<GCOperationManager*, max_handlers> handlers;

public:
    GC()
    {
        // FIX: getData should not return a const uint8*... should it?
        addfree(buffer.getData(), size);
    }


    template <class T>
    GCPointer3<T> alloc2(size_t count = 1)
    {
        GCPointer3<T> gcp(*this);

        alloc(&gcp.gco, sizeof(T) * count);

        return gcp;
    }

    template <class T>
    GCPointer3<T> auto_ptr(GCObject& gco)
    {
        GCPointer3<T> gcp(*this);
        gcp.gco = gco;
        return gcp;
    }
};

// GC Handles are OPTIONAL, and are not preferred (copying GCObjects around is ideal)
// - but acceptable and sometimes necessary
typedef uint8_t gc_handle_t;


class GCPoolManagerBase
{
protected:
    template <class T, bool (*item_is_unallocated)(T&)>
    static int alloc(T* pool_array, gc_handle_t array_size, const T& new_item)
    {
        for(gc_handle_t i = 0; i < array_size; i++)
        {
            T& g = pool_array[i];
            if(item_is_unallocated(g))
            {
                g = new_item;
                return i;
            }
        }

        return -1;
    }
};


template <class T>
struct gc_pool_traits;


template<>
struct gc_pool_traits<GCObject>
{
    static bool is_free(GCObject& gco) { return gco.data == nullptr; }
    static void free(GCObject& gco) { gco.data = nullptr; }
};

template <class T, class TPoolTraits = gc_pool_traits<T>>
class GCPoolManager : public GCPoolManagerBase
{
protected:
    typedef T value_type;
    typedef TPoolTraits pool_traits;

    GCObject gc_pool_array;

    gc_handle_t array_size() { return gc_pool_array.size() / sizeof(value_type); }


public:
    // Start with room for 6 items, but we can grow it if needed
    // NOTE that although we can kinda shrink it, THIS is somewhat
    // subject to fragmentation.  Yet another reason to avoid handles
    // if possible
    GCPoolManager() : gc_pool_array(_gc.alloc(sizeof(value_type) * 6))
    {
    }


    /**
     * Copies specified gco into our global tracker and spits out a global-friendly
     * handle
     * @brief alloc
     * @param gco
     * @return
     */
    gc_handle_t alloc(T& new_item)
    {
        auto array = reinterpret_cast<value_type*>(_gc.lock(gc_pool_array));
        int handle = GCPoolManagerBase::alloc<value_type, pool_traits::is_free>(array, array_size(), new_item);
        if(handle == -1)
        {
            // if we're out of handles, grow it and try again
            _gc.grow(gc_pool_array, sizeof(value_type) * 4);
        }

        _gc.unlock(gc_pool_array);

        return handle;
    }


    void free(gc_handle_t handle)
    {
        auto array = static_cast<value_type*>(_gc.lock(gc_pool_array));

        pool_traits::free(array[handle]);

        _gc.unlock(gc_pool_array);
    }
};

// TODO: need special handler which if memory is moved for this pool,
// GCObject*->next pointers get updated appropriately
class GCHandleManager : public GCPoolManager<GCObject>
{
public:
};


class GCHandleOperationManager : public GCOperationManager
{
public:
    virtual void lock(GCObject& gco)
    {

    }

    virtual void move(GCObject& gco)
    {
        // TODO: ensure
        // GCObject*->next pointers get updated appropriately
    }

    virtual void housekeep(GCObject& gco)
    {

    }
};


template <class T>
struct GCAllocator
{
    class GCPointer
    {
        GCObject gco;

    public:
        typedef T   value_type;

        GCPointer(GCObject gco) : gco(gco)
        {
        }

        const T* lock() { return _gc.lock(gco); }
        void unlock()   { _gc.unlock(gco); }
        operator const T* () { return _gc.lock(gco); }
    };

    typedef GCPointer        pointer;
    typedef const pointer   const_pointer;
    typedef T               value_type;

    // FIX: Need a GCObject pointer wrapper for this to work right (and still maybe not work right even then)
    pointer allocate(size_t n)
    {
        return _gc.alloc(n);
    }

    void deallocate(pointer p, size_t)
    {
        _gc._free(p);
    }
};


#include "GC.hpp"


}

namespace std { namespace experimental {

template <>
struct allocator_traits<::FactUtilEmbedded::experimental::GCAllocator<void*>>
{

};


}}
}

