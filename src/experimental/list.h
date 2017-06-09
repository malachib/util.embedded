#pragma once

#include "../LinkedList.h"

namespace FactUtilEmbedded { namespace std {

namespace experimental {

template <class TNode>
struct node_next_extractor
{
    static TNode* getNext(TNode* current);
};


template <>
struct node_next_extractor<SinglyLinkedNode>
{
    static inline SinglyLinkedNode* getNext(SinglyLinkedNode* current) { return current->getNext(); }
};

template <class TNode = SinglyLinkedNode>
class special_singly_forward_iterator
{
protected:
    typedef TNode node_type;

    node_type* current;

    node_type* getNext() const { return node_next_extractor<TNode>::getNext(current); }

    void advance()
    {
        current = getNext();
    }

    special_singly_forward_iterator(node_type* node) : current(node) {}

public:

    node_type* getCurrent() const { return current; }
};


template <class T, class TNode = SinglyLinkedNode>
struct node_allocator
{
public:
    typedef T value_type;
    typedef TNode node_type;

    node_type* allocate(value_type *reference)
    {
        return reference;
    }

    void deallocate(node_type* node)
    {
        // Maybe setting node->next to null here would be prudent?
    }

    // hint helps us track down what the associated value is
    // perhaps the node * is not in our control
    static value_type* get_associated_value(node_type* node, const void* hint)
    {
        return static_cast<value_type*>(node);
    }
};

template<class T>
class list
{

};

struct forward_iterator_tag {};

struct InputIterator {};


template <class TNodeAllocator, const void* hint = nullptr>
struct ForwardIteratorBase : public special_singly_forward_iterator<typename TNodeAllocator::node_type>
{
    typedef typename TNodeAllocator::value_type value_type;
    typedef typename TNodeAllocator::node_type node_type;
    typedef special_singly_forward_iterator<node_type> base_t;

    ForwardIteratorBase(const ForwardIteratorBase& source) : base_t(source.getCurrent()) {}

    ForwardIteratorBase(node_type* node) : base_t(node) {}


    operator value_type* () const
    {
        return TNodeAllocator::get_associated_value(base_t::getCurrent(), hint);
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

class forward_list_base
{
public:
    typedef SinglyLinkedNode node_type;
    typedef SinglyLinkedList list_type;

protected:
    list_type list;

public:
    bool empty() { return list.getHead(); }
};

// NOTE: Not sure what to do about std::initializer_list
// It's a "special" class which the compiler knows about and depends on.  Now sure if I
// can interact with it/make my own(that will get picked up) in an embedded environment
template <class T, class TNodeAllocator = node_allocator<T>>
class forward_list : public forward_list_base
{
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;

    struct ForwardIterator : public ForwardIteratorBase<TNodeAllocator>
    {
        typedef ForwardIteratorBase<TNodeAllocator> base;

        ForwardIterator(const ForwardIterator& source) :
            base(source)
        {
        }

        ForwardIterator(node_type* node) :
            base(node)
        {
        }


        ForwardIterator& operator++()
        {
            base::advance();
            return *this;
        }

        // postfix version
        ForwardIterator operator++(int)
        {
            ForwardIterator temp(*this);
            operator++();
            return temp;
        }
    };

    typedef ForwardIterator         iterator;
    typedef const ForwardIterator   const_iterator;

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
};

}

}}
