#pragma once

#include "../LinkedList.h"

namespace FactUtilEmbedded { namespace std {

namespace experimental {

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

    SinglyLinkedNode* get_associated_node(T* reference, const void* hint) { return reference; }
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

template <class T, class TNodeAllocator = node_allocator<T>>
class forward_list
{
    typedef SinglyLinkedNode node_type;
    typedef T                value_type;

    struct ForwardIterator
    {
        node_type* current;

        ForwardIterator(const SinglyLinkedList& ll)
        {
            current = ll.getHead();
        }

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

        value_type* getCurrent() const
        {
            TNodeAllocator allocator;
            return allocator.get_associated_value(current, nullptr);
        }


        // cast operator
        operator value_type* ()
        {
            return getCurrent();
        }
    };

    typedef ForwardIterator         iterator;
    typedef const ForwardIterator   const_iterator;

    SinglyLinkedList list;

public:
    iterator begin() { return iterator(list.getHead()); }
    iterator end() { return iterator(nullptr); }

    // not a const like in standard because we expect to actually modify
    // the prev/next parts of value
    void push_front(value_type& value)
    {
        TNodeAllocator allocator;
        node_type* node = allocator.allocate(&value);
        //node_type* node = TNodeAllocator::allocate(&value);

        list.insertAtBeginning(node);
    }


    iterator insert_after(const_iterator pos, value_type& value)
    {
        TNodeAllocator allocator;
        //value_type* pos_value = allocator.get_associated_value(pos.getCurrent(), nullptr);
        value_type* pos_value = pos;
        node_type* pos_node = allocator.get_associated_node(pos_value, nullptr);
        node_type* node = allocator.allocate(&value);

        // FIX: insertBetween is overcompliated, the insert_after is cleaner and better
        // (the getNext() is always the value used, so why bother making it an explicit param)
        list.insertBetween(pos_node, pos_node->getNext(), node);

        return iterator(node);
    }

    iterator erase_after(const_iterator pos)
    {
        TNodeAllocator allocator;
        value_type* pos_value = pos;
        node_type* pos_node = allocator.get_associated_node(pos_value, nullptr);

        node_type* node_to_erase = pos_node->getNext();

        pos_node->removeNext();
        allocator.deallocate(node_to_erase);
        return iterator(pos_node->getNext());
    }
};

}

}}
