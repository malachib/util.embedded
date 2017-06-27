#pragma once

#include "../LinkedList.h"
#include "memory.h"

namespace FactUtilEmbedded { namespace std {

namespace experimental {

template <class TNode> struct node_traits;

template <>
struct node_traits<SinglyLinkedNode>
{
    typedef SinglyLinkedNode node_type;
    typedef SinglyLinkedList list_type;

    static node_type* null_node() { return nullptr; }
    static node_type* get_next(node_type* node) { return node->getNext(); }
    static node_type* get_head(list_type* list) { return list->getHead(); }
};

template <>
struct node_traits<DoublyLinkedNode>
{
    typedef DoublyLinkedNode node_type;
    typedef DoublyLinkedList list_type;

    static node_type* null_node() { return nullptr; }
    static node_type* get_next(node_type* node) { return node->getNext(); }
    static node_type* get_prev(node_type* node) { return node->getPrev(); }
    static node_type* get_head(list_type* list) { return list->getHead(); }
};



template <class TNode, class TAllocatorTraits = allocator_traits<Allocator<TNode>>>
class node_pointer
{
protected:
    typedef TNode node_type;
    typedef TAllocatorTraits node_allocator_traits;
    typedef node_pointer<node_type> this_t;
    typedef typename node_allocator_traits::pointer _node_pointer;

    _node_pointer current;

    node_pointer(node_type* current) : current(current) {}

public:
    _node_pointer getCurrent() const { return current; }

    bool operator==(const this_t& rhs)
    {
        return current == rhs.getCurrent();
    }

    bool operator!=(const this_t& rhs)
    {
        return current != rhs.getCurrent();
    }
};





template <class T, class TNode,
        class TNodeAllocatorTraits = allocator_traits<Allocator<TNode>>>
struct node_allocator
{
public:
    typedef T value_type;
    typedef TNode node_type;
    typedef TNodeAllocatorTraits node_allocator_traits;
    typedef allocator_traits<Allocator<value_type>> value_allocator_traits;

    typedef typename node_allocator_traits::pointer node_pointer;
    typedef typename value_allocator_traits::pointer value_pointer;

    typedef typename value_allocator_traits::const_void_pointer const_void_pointer;

    node_pointer allocate(value_pointer reference)
    {
        return reference;
    }

    void deallocate(node_pointer node)
    {
        // Maybe setting node->next to null here would be prudent?
    }

    // hint helps us track down what the associated value is
    // perhaps the node * is not in our control
    static value_pointer get_associated_value(node_pointer node, const_void_pointer hint)
    {
        return static_cast<value_pointer>(node);
    }
};


struct InputIterator {};


template <class TNodeAllocator, const void* hint = nullptr,
        class TBase = node_pointer<typename TNodeAllocator::node_type>>
struct OutputIterator : public TBase
{
    typedef typename TNodeAllocator::value_type value_type;
    typedef typename TNodeAllocator::node_type node_type;
    typedef TBase base_t;

    OutputIterator(node_type* node) : base_t(node) {}


    // FIX: doing for(auto i : list) seems to do a *copy* operation
    // for(value_type& i : list) is required to get a reference.  Check to see if this is
    // proper behavior
    value_type& operator*()
    {
        return *TNodeAllocator::get_associated_value(base_t::getCurrent(), hint);
    }
};


class forward_list_base
{
public:
    typedef SinglyLinkedNode node_type;
    typedef SinglyLinkedList list_type;

protected:
    list_type list;

    node_type* pop_front()
    {
        node_type* node = list.getHead();
        list.experimental_set_head(node->getNext());
        return node;
    }

public:
    bool empty() { return list.getHead() == nullptr; }

};


class list_base
{
public:
    typedef DoublyLinkedNode node_type;
    typedef DoublyLinkedList list_type;

protected:
    list_type list;

    node_type* pop_front()
    {
        node_type* node = list.getHead();
        list.experimental_set_head(node->getNext());
        return node;
    }

public:
    bool empty() { return list.getHead() == nullptr; }

};


template <class TNodeAllocator, class TBase = node_pointer<typename TNodeAllocator::node_type>>
struct ForwardIterator : public OutputIterator<TNodeAllocator, nullptr, TBase>
{
    typedef typename TNodeAllocator::node_type   node_type;
    typedef typename TNodeAllocator::value_type  value_type;
    typedef OutputIterator<TNodeAllocator, nullptr, TBase> base_t;

    ForwardIterator(const ForwardIterator& source) :
            base_t(source)
    {
    }

    ForwardIterator(node_type* node) :
            base_t(node)
    {
    }


    ForwardIterator& operator++()
    {
        base_t::current = node_traits<node_type>::get_next(base_t::current);
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


template <class TNodeAllocator, class TBase = node_pointer<typename TNodeAllocator::node_type>>
struct BidirectionalIterator : public ForwardIterator<TNodeAllocator, TBase>
{
    typedef typename TNodeAllocator::node_type   node_type;
    typedef typename TNodeAllocator::value_type  value_type;
    typedef ForwardIterator<TNodeAllocator, TBase> base_t;

    BidirectionalIterator(const BidirectionalIterator& source) :
        base_t(source)
    {
    }

    BidirectionalIterator(node_type* node) :
        base_t(node)
    {
    }


    BidirectionalIterator& operator--()
    {
        base_t::current = node_traits<node_type>::get_prev(base_t::current);
        return *this;
    }

    // postfix version
    BidirectionalIterator operator--(int)
    {
        BidirectionalIterator temp(*this);
        operator--();
        return temp;
    }
};


template <class TNodeAllocator, class TIterator>
class list_base2
{
public:
    typedef typename TNodeAllocator::node_type node_type;
    typedef node_traits<node_type> node_traits_t;
    typedef typename node_traits_t::list_type list_type;
    typedef typename TNodeAllocator::value_type value_type;
    typedef typename TNodeAllocator::node_pointer node_pointer;
    typedef typename TNodeAllocator::value_pointer value_pointer;

    typedef TIterator        iterator;
    typedef const iterator   const_iterator;

    typedef value_type&     reference;

protected:
    list_type list;
    TNodeAllocator node_allocator;

    TNodeAllocator& get_node_allocator()
    {
        return node_allocator;
    }

    inline node_pointer get_head()
    {
        return node_traits_t::get_head(&list);
    }

    node_type* _pop_front()
    {
        node_pointer node = get_head();
        list.experimental_set_head(node_traits_t::get_next(node));
        return node;
    }

public:
    bool empty() { return get_head() == nullptr; }

    iterator begin() { return iterator(get_head()); }

    reference front() { return *begin(); }

    // not a const like in standard because we expect to actually modify
    // the prev/next parts of value
    void push_front(value_type& value)
    {
        node_pointer node = get_node_allocator().allocate(&value);

        list.insertAtBeginning(node);
    }


    void pop_front()
    {
        node_type* node = _pop_front();
        get_node_allocator().deallocate(node);
    }
};


// NOTE: Not sure what to do about std::initializer_list
// It's a "special" class which the compiler knows about and depends on.  Now sure if I
// can interact with it/make my own(that will get picked up) in an embedded environment
template <class T, class TNodeAllocator = node_allocator<T, SinglyLinkedNode>>
class forward_list :
        public forward_list_base
{
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;

protected:
    TNodeAllocator node_allocator;

    TNodeAllocator& get_node_allocator()
    {
        return node_allocator;
    }

public:
    typedef ForwardIterator<TNodeAllocator>         iterator;
    typedef const iterator   const_iterator;

    iterator begin() const { return iterator(list.getHead()); }
    iterator end() const { return iterator(nullptr); }

    // not a const like in standard because we expect to actually modify
    // the prev/next parts of value
    void push_front(value_type& value)
    {
        node_type* node = get_node_allocator().allocate(&value);

        list.insertAtBeginning(node);
    }


    void pop_front()
    {
        node_type* node = forward_list_base::pop_front();
        get_node_allocator().deallocate(node);
    }


    iterator insert_after(const_iterator pos, value_type& value)
    {
        node_type* pos_node = pos.getCurrent();
        node_type* node = get_node_allocator().allocate(&value);

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
        get_node_allocator().deallocate(node_to_erase);
        return iterator(pos_node->getNext());
    }

    reference front() { return *begin(); }

    // non-standard:
    // removes/deallocate node at pos and splices in value
    const_iterator replace_after(const_iterator pos, value_type& value)
    {
        auto node_allocator = get_node_allocator();

        node_type* pos_node = pos.getCurrent();
        // old 'next' node prep it for erase
        node_type* node_to_erase = pos_node->getNext();
        // new 'next' node allocate node portion, if necessary
        node_type* new_node = node_allocator.allocate(&value);

        // TODO: set pos_node->next to be &value
        // inset new 'next' node after current node and before old 'next''s next
        // node
        new_node->insertBetween(pos_node, node_to_erase->getNext());

        //
        node_allocator.deallocate(node_to_erase);

        return pos;
    }


    // Non-standard
    void replace_front(value_type& value)
    {
        auto node_allocator = get_node_allocator();

        node_type* front_node = list.getHead();

        node_type* new_front_node = node_allocator.allocate(value);

        new_front_node->insertBetween(nullptr, front_node->getNext());

        list.experimental_set_head(new_front_node);

    }


    // Non-standard, eliminate this call in favor of more manual pop_front/etc
    void remove(reference r)
    {
        list.remove(&r);
    }

#ifdef UNUSEDXXX
    // Non-standard, eliminate this call in favor of more manual pop_front/etc
    void insert(node_type* referenceNode, node_type* nodeToInsert)
    {
        list.insert(referenceNode, nodeToInsert);
    }

    // Non-standard, eliminate this call
    // (adds to end)
    void add(node_type* node)
    {
        list.add(node);
    }
#endif
};



template <class T, class TNodeAllocator = node_allocator<T, DoublyLinkedNode>>
class list :
        public list_base2<TNodeAllocator, BidirectionalIterator<TNodeAllocator>>
{
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;

    typedef list_base2<TNodeAllocator, BidirectionalIterator<TNodeAllocator>> base_t;
    typedef typename base_t::node_traits_t  node_traits_t;

    typedef typename base_t::node_type      node_type;
    typedef typename base_t::node_pointer   node_pointer;

    TNodeAllocator& get_node_allocator()
    {
        return base_t::node_allocator;
    }

public:
    typedef BidirectionalIterator<TNodeAllocator>         iterator;
    typedef const iterator   const_iterator;

    iterator end() { return iterator(nullptr); }

    // not a const like in standard because we expect to actually modify
    // the prev/next parts of value
    void push_front(reference value)
    {
        node_pointer node = get_node_allocator().allocate(&value);

        base_t::list.insertAtBeginning(node);
    }


    /*
    void pop_front()
    {
        // FIX: almost definitely this isn't gonna work right (not accounting for tail)
        node_type* node = base_t::pop_front();
        get_node_allocator().deallocate(node);
    } */


    iterator insert_after(const_iterator pos, reference value)
    {
        node_type* pos_node = pos.getCurrent();
        node_type* node = get_node_allocator().allocate(&value);

        // FIX: insertBetween is overcompliated, the insert_after is cleaner and better
        // (the getNext() is always the value used, so why bother making it an explicit param)
        base_t::list.insertBetween(pos_node, pos_node->getNext(), node);

        return iterator(node);
    }

    iterator erase_after(const_iterator pos)
    {
        node_pointer pos_node = pos.getCurrent();
        node_pointer node_to_erase = pos_node->getNext();

        pos_node->removeNext();
        get_node_allocator().deallocate(node_to_erase);
        return iterator(pos_node->getNext());
    }
};

}

}}
