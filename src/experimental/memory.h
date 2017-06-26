#pragma once

namespace FactUtilEmbedded {
namespace std {
namespace experimental {

// 100% generic allocator
template <class T>
struct Allocator
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef void* void_pointer;
    typedef const void* const_void_pointer;
};

template <class TAllocator>
struct allocator_traits
{
    typedef TAllocator                          allocator_type;
    typedef typename TAllocator::value_type     value_type;
    typedef typename TAllocator::pointer        pointer;
    typedef size_t                              size_type;

    typedef typename allocator_type::const_void_pointer     const_void_pointer;

    static pointer allocate(allocator_type& a, size_type n)
    {
        return a.allocate(n);
    }

    static void deallocate(allocator_type& a, size_type n)
    {
        return a.deallocate(n);
    }
};


template <class TPtr> struct pointer_traits;
template <class T> struct pointer_traits<T*>;


}}}