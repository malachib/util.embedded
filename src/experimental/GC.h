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

    ptr_t data;
    size_t size;

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
};

template <size_t size>
class GC
{
    layer1::MemoryContainer<size> buffer;

    SinglyLinkedList free;
    SinglyLinkedList allocated;

    size_t used() const
    {
        auto gcobjects = static_cast<GCObject*>(buffer.getData());

        while(gcobjects->is_free())
        {
            gcobjects++;
        }
    }

    void addfree(uint8_t* location, size_t len)
    {
        // FIX: We're burning up space in free memory by tracking
        // this 'self' pointer.  Not needed, but using anyway
        // *might* be useful if we decide to
        // store free GCObjects elsewhere - though unlikely we'll do that
        GCObject* free_gco = static_cast<GCObject*>((void*)location);

        free_gco->data = location;
        free_gco->size = len;

        free.insertAtBeginning(free_gco);
    }

    uint8_t* __alloc(size_t len)
    {
        //auto node = free.getHead();
        layer1::LinkedListIterator<GCObject> i(free);

        // TODO: overload i bool operator so that
        // we can properly do while(i)
        while(i() != nullptr)
        {
            GCObject* node = i.getCurrent();
            if(node->size >= len)
            {
                free.remove(node);
                return node->data;
            }
        }

        return nullptr;
    }

public:
    GC()
    {
    }

    size_t available() const
    {

    };


    GCObject alloc(size_t len)
    {
        GCObject gco;

        gco.data = __alloc(gco.size);

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
