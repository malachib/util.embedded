#pragma once

#include "../fact/buffer.h"
#include "../LinkedList.h"

// The point of the GC experiment is not automatic freeing,
// but rather gaurunteed fragmentation avoidance

namespace FactUtilEmbedded { namespace experimental {

class GCObject : public SinglyLinkedNode
{
    // NOTE: probably is a performance penalty, not aligning
    // to word boundaries.  If we like the GC overall,
    // revisit this
    typedef uint8_t* ptr_t;
public:
    // TODO: Put this behavior into underlying SinglyLinkedNode
    GCObject() { next = nullptr; }

    ptr_t data;
    size_t size;

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

template <size_t size>
class GC
{
#ifdef UNIT_TEST
public:
#endif
    layer1::MemoryContainer<size> buffer;

    typedef GCObject free_gco_t;

    SinglyLinkedList free;
    SinglyLinkedList allocated;

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

    void addfree(const uint8_t* location, size_t len)
    {
        // FIX: We're burning up space in free memory by tracking
        // this 'self' pointer.  Not needed, but using anyway
        // *might* be useful if we decide to
        // store free GCObjects elsewhere - though unlikely we'll do that
        //GCObject* free_gco = static_cast<GCObject*>((void*)location);
        GCObject* free_gco = new ((void*)location) GCObject();

        free_gco->data = (uint8_t*)location;
        free_gco->size = len;

        /*
         * Beginning code to add back free slot into
         * ordered location within linkedlist
        layer1::LinkedListIterator<GCObject> i(free);

        while(i())
        {
            if(i.getCurrent()->data > location);
            {
                free.insert(i.getCurrent(), free_gco);
                return;
            }

            if(!i.getNext())
            {
                //i.getCurrent()->insertBetween(i, nullptr);
                // TODO: add in an insertAfter into SinglyLinkedList
                free.insertBetween(i, nullptr, free_gco);
            }

            i++;
        } */


        free.insertAtBeginning(free_gco);
    }

    uint8_t* __alloc(size_t& len)
    {
        //auto node = free.getHead();
        layer1::LinkedListIterator<GCObject> i(free);

        // TODO: overload i bool operator so that
        // we can properly do while(i)
        while(i() != nullptr)
        {
            GCObject* node = i.getCurrent();
            if(node->size > len)
            {
                free.remove(node);
                // If free memory chunk size - request len size
                // < size of new free_gco_t to be created,
                // then we expand our request length slightly
                // and burn up that memory.  Otherwise,
                // we use leftover memory and create a new
                // free_gco_t in there
                if((node->size - len) < sizeof(free_gco_t))
                {
                    len = node->size;
                }
                else
                {
                    addfree(node->data + len, node->size - len);
                }
                return node->data;
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


    GCObject alloc(size_t len)
    {
        GCObject gco;

        gco.data = __alloc(len);

        if(gco.data)
        {
            allocated.insertAtBeginning(&gco);
            gco.size = len;
        }

        return gco;
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

#include "GC.hpp"

}}
