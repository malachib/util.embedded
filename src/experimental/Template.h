#pragma once

#include "../fact/ostream.h"
#include "../fact/istream.h"
#include "../fact/buffer.h"
#include "list.h"

// Macro-expansion templating

namespace FactUtilEmbedded { namespace experimental {

// This is more or less like a mailbox arrangement in embedded
// systems, however once we use GC with it its character will
// change
class StreamPipeline
{
    typedef FactUtilEmbedded::layer3::MemoryContainer<> buf_t;

    struct MiniBuffer : public SinglyLinkedNode,
            public buf_t
    {
        MiniBuffer(void* const buffer, size_t len) : buf_t(buffer, len) {}
    };

    typedef FactUtilEmbedded::std::experimental::forward_list<MiniBuffer> buffers_t;

    buffers_t buffers;
    buffers_t::iterator last_item;

public:
    StreamPipeline() : last_item(buffers.end()) {}

    void write(const void* buffer, size_t len)
    {
        // FIX: use GC memory allocator instead once it's more reliable
        // FIX: Once above is available, we should be able to directly use GC buffer reference
        // instead of copying buffer/len.
        auto miniBuffer = new MiniBuffer((void* const)buffer, len);
        if(last_item == buffers.end())
        {
            buffers.push_front(*miniBuffer);
            last_item = buffers.begin();
        }
        else
            last_item = buffers.insert_after(last_item, *miniBuffer);
    }

    buf_t read()
    {
        // TODO: update last_item
        // TODO: consider just using a queue instead
        // TODO: consider making queue std::ish
        MiniBuffer& buffer = buffers.front();
        buffers.pop_front();
        buf_t copy = buffer;
        delete &buffer;
        return copy;
    }
};

// TODO: If needed, link multiple stream processors together(linked list)
class StreamProcessor
{
    typedef FactUtilEmbedded::std::istream istream_t;
    typedef FactUtilEmbedded::std::ostream ostream_t;

public:
    enum ProcessState
    {
        Done,
        InputBlocking,
        OutputBlocking,
        LongRunning,
        Error
    };

    // process input until we block on something, or finish, then return
    virtual ProcessState process(StreamPipeline& pin, StreamPipeline& pout) = 0;

    ProcessState process(istream_t& cin, ostream_t& cout)
    {
        StreamPipeline pin;
        StreamPipeline pout;

        ProcessState result = process(pin, pout);
    }
};



}}
