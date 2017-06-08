#pragma once

#include "../fact/buffer.h"
#include "../LinkedList.h"

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
    size_t size;
    bool pinned;

    // Shrinks free_gco by a specified amount, moving pointer upwards
    void shrink_up(size_t size)
    {
        data += size;
        this->size -= size;
    }


    // Shrinks free_gco by a specified amount, leaving pointer intact
    void shrink_down(size_t size)
    {
        this->size -= size;
    }

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

class GC_base
{
protected:
#ifdef UNIT_TEST
public:
#endif

    typedef GCObject free_gco_t;
    typedef layer1::LinkedListIterator<GCObject> ll_iterator_t;
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

            printf("Node: addr = %p, size = %lu, next = %p\r\n", current->data, current->size, current->getNext());
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

        if(gco_primary->data + gco_primary->size != gco_secondary->data)
            printf("recombine failure: incorrect sizes\r\n");
#endif

        gco_primary->size += gco_secondary->size;
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

            if(current->data + current->size == next->data)
            {
                recombine(current, next);
                walk_free("recombined");
            }

            i++;
        }
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
        free_gco->size = len;

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
            if(location + free_gco->size == next->data)
            {
                recombine(free_gco, next);
                walk_free("recombined");
            }
//#ifdef DEBUG
            else if(location + free_gco->size > next->data)
            {
                // ERROR: free memory spots should not overlap

            }
//#endif
        }
    }

    void _free(GCObject& gco)
    {
        addfree(gco.data, gco.size);
        allocated.remove(&gco);
    }


    uint8_t* lock(GCObject& gco)
    {
        return gco.data;
    }


    void unlock(GCObject& gco)
    {

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
    //uint16_t refCount = 0;

public:
    GC_base& gc;
    GCObject gco;

    GCPointer3(GC_base& gc) : gc(gc) {}

    T* lock() { return (T*)gc.lock(gco); }
    void unlock()   { gc.unlock(gco); }
    operator T* ()  { return gc.lock(gco); }
};


template <size_t size>
class GC : public GC_base
{
#ifdef UNIT_TEST
public:
#endif
    layer1::MemoryContainer<size> buffer;

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
            if(node->size > len)
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
                if((node->size - len) < sizeof(free_gco_t))
                {
                    len = node->size;

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
                    location += node->size - len;
                    node->shrink_down(len);
#else
                    // re-add the node with adjusted parameters
                    addfree(node->data + len, node->size - len);
#endif
                }

                // return the previous free node data ptr
                return location;
            }
            else if(node->size == len)
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
            total += i.getCurrent()->size;
            i++;
        }

        return total;
    }


    void defragment()
    {

    }

public:
    GC()
    {
        // FIX: getData should not return a const uint8*... should it?
        addfree(buffer.getData(), size);
    }

    size_t available() const
    {
        layer1::LinkedListIterator<GCObject> i(free);

        size_t total = 0;

        while(i())
        {
            total += i.getCurrent()->size;
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
            gco->size = len;
        }
    }

    GCObject alloc(size_t len)
    {
        GCObject gco;
        alloc(&gco, len);
        return gco;
    }


    template <class T>
    GCPointer3<T> alloc2(size_t count = 1)
    {
        GCPointer3<T> gcp(*this);

        alloc(&gcp.gco, sizeof(T) * count);

        return gcp;
    }
};




#include "GC.hpp"

}}
