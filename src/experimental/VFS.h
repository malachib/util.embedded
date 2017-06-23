#pragma once

#include "../fact/iostream.h"
#include "../fact/buffer.h"

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
    virtual bool exists(TPath path) = 0;

    virtual FactUtilEmbedded::std::basic_iostream<char>& open(TPath path,
                                                              ios_base::openmode openmode = ios_base::in) = 0;
    virtual FactUtilEmbedded::std::ostream& create(TPath path) = 0;

    virtual void close(ios_base& stream) = 0;
};


template <class TKey, class TMeta>
class Factory
{
public:
    virtual bool contains(TKey key, TMeta meta = nullptr) = 0;

    virtual bool create(TKey key, TMeta meta = nullptr) = 0;

    //template <class TOptions>
    //virtual bool create(TKey key, TOptions options, TMeta meta = nullptr) = 0;
};

template <class TKey, class TMeta>
class AggregateFactory : public Factory<TKey, TMeta>
{
    FactUtilEmbedded::layer3::Array<Factory<TKey, TMeta>*> factories;

public:
    virtual bool contains(TKey key, TMeta meta = nullptr)
    {
        for(auto f : factories)
        {
            if(f->contains(key))
            {

            }
        }
    }

};


template <typename TPath>
class AggregateVFS : public VFS<TPath>
{
    FactUtilEmbedded::layer3::Array<VFS<TPath>*>& vfs_list;

    typedef FactUtilEmbedded::std::ios_base ios_base;

public:
    AggregateVFS(FactUtilEmbedded::layer3::Array<VFS<TPath>*>& vfs_list) : vfs_list(vfs_list) {}

    // TODO: split out Aggregator logic ala apprentice style Factory aggregator
    virtual bool exists(TPath path)
    {
        for(auto vfs : vfs_list)
        {
            vfs->exists(path);
        }
    }

    virtual FactUtilEmbedded::std::basic_iostream<char>& open(TPath path,
                                                              ios_base::openmode openmode = ios_base::in)
    {

    }

    virtual FactUtilEmbedded::std::ostream& create(TPath path)
    {

    }

    virtual void close(ios_base& stream)
    {

    }
};






template <typename TPath = const char*>
class MemoryVFS : public VFS<TPath>
{
    typedef FactUtilEmbedded::std::ios_base ios_base;

public:
    virtual bool exists(TPath path);

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