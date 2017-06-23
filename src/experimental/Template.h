#pragma once

#include "../fact/ostream.h"
#include "../fact/istream.h"
#include "../fact/buffer.h"
#include "list.h"

// Macro-expansion templating

namespace FactUtilEmbedded { namespace experimental {

class StreamPipeline
{
    typedef FactUtilEmbedded::layer3::MemoryContainer<> buf_t;

    struct MiniBuffer : public SinglyLinkedNode,
            buf_t
    {
    };

    FactUtilEmbedded::std::experimental::forward_list<MiniBuffer> buffers;
public:

    void write(const uint8_t* buffer, size_t len)
    {

    }

    buf_t read()
    {
        MiniBuffer buffer = buffers.front();
        buffers.pop_front();
        return buffer;
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
