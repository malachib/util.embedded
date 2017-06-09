#pragma once

#include "../LinkedList.h"

namespace FactUtilEmbedded { namespace std {

namespace experimental {


class special_singly_forward_iterator
{
protected:
    SinglyLinkedNode* current;

public:
    SinglyLinkedNode* getCurrent() const { return current; }
};

template <class T>
struct node_allocator
{
public:
    SinglyLinkedNode* allocate(T *reference)
    {
        return reference;
    }

    void deallocate(SinglyLinkedNode* node)
    {
        // Maybe setting node->next to null here would be prudent?
    }

    T* get_associated_value(SinglyLinkedNode* node, const void* hint)
    {
        return static_cast<T*>(node);
    }
};

template <>
struct node_allocator<SinglyLinkedNode>
{
public:
};

template<class T>
class list
{

};

struct forward_iterator_tag {};

struct InputIterator {};

// NOTE: Not sure what to do about std::initializer_list
// It's a "special" class which the compiler knows about and depends on.  Now sure if I
// can interact with it/make my own(that will get picked up) in an embedded environment
template <class T, class TNodeAllocator = node_allocator<T>>
class forward_list
{
    typedef SinglyLinkedNode node_type;
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;

    struct ForwardIterator : public special_singly_forward_iterator
    {
        ForwardIterator(const ForwardIterator& source)
        {
            current = source.getCurrent();
        }

        ForwardIterator(node_type* node)
        {
            current = node;
        }


        ForwardIterator& operator++()
        {
            current = current->getNext();
            return *this;
        }

        // postfix version
        ForwardIterator operator++(int)
        {
            ForwardIterator temp(*this);
            operator++();
            return temp;
        }

        // cast operator
        operator value_type* () const
        {
            TNodeAllocator allocator;
            return allocator.get_associated_value(current, nullptr);
        }

        /*
        friend bool operator==(const ForwardIterator& lhs, const ForwardIterator& rhs)
        {
            return *((value_type*)lhs) == *((value_type*)rhs);
        }

        friend bool operator!=(const ForwardIterator& lhs, const ForwardIterator& rhs)
        {
            return !(lhs == rhs);
        } */
    };

    typedef ForwardIterator         iterator;
    typedef const ForwardIterator   const_iterator;

    SinglyLinkedList list;

    TNodeAllocator node_allocator;

public:
    iterator begin() { return iterator(list.getHead()); }
    iterator end() { return iterator(nullptr); }

    // not a const like in standard because we expect to actually modify
    // the prev/next parts of value
    void push_front(value_type& value)
    {
        node_type* node = node_allocator.allocate(&value);

        list.insertAtBeginning(node);
    }


    iterator insert_after(const_iterator pos, value_type& value)
    {
        node_type* pos_node = pos.getCurrent();
        node_type* node = node_allocator.allocate(&value);

        // FIX: insertBetween is overcompliated, the insert_after is cleaner and better
        // (the getNext() is always the value used, so why bother making it an explicit param)
        list.insertBetween(pos_node, pos_node->getNext(), node);

        return iterator(node);
    }

    iterator erase_after(const_iterator pos)
    {
        node_type* pos_node = pos.getCurrent();
        node_type* node_to_erase = pos_node->getNext();

        pos_node->removeNext();
        node_allocator.deallocate(node_to_erase);
        return iterator(pos_node->getNext());
    }

    reference front() { return *begin(); }

    bool empty() { return list.getHead(); }
};

}

}}
