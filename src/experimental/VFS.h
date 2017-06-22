#pragma once

#include "../fact/iostream.h"

namespace FactUtilEmbedded { namespace experimental {

namespace layer3 {

template<class TFileProvider, typename TPath = const char *>
class VFS
{
    typedef FactUtilEmbedded::std::ios_base ios_base;
    TFileProvider &fileProvider;

public:
    VFS(TFileProvider &fileProvider) : fileProvider(fileProvider) {}

    FactUtilEmbedded::std::basic_iostream<char> &open(TPath path, ios_base::openmode openmode);
    FactUtilEmbedded::std::ostream &create(TPath path);

    void close(ios_base &stream);
};

}

namespace layer5 {

template <typename TPath = const char*>
class VFS
{
    typedef FactUtilEmbedded::std::ios_base ios_base;

public:
    virtual FactUtilEmbedded::std::basic_iostream<char>& open(TPath path,
                                                              ios_base::openmode openmode = ios_base::in) = 0;
    virtual FactUtilEmbedded::std::ostream& create(TPath path) = 0;

    virtual void close(ios_base& stream) = 0;
};


template <typename TPath = const char*>
class MemoryVFS : public VFS<TPath>
{
    typedef FactUtilEmbedded::std::ios_base ios_base;

public:
    virtual FactUtilEmbedded::std::basic_iostream<char>& open(TPath path,
                                                              ios_base::openmode openmode = ios_base::in);
    virtual FactUtilEmbedded::std::ostream& create(TPath path);

    virtual void close(ios_base& stream);
};

template <>
FactUtilEmbedded::std::basic_iostream<char>&
    MemoryVFS<const char*>::open(const char* name, ios_base::openmode mode);

}

}}