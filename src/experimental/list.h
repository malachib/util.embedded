#pragma once

#include "../LinkedList.h"

namespace FactUtilEmbedded { namespace std {

namespace experimental {

// NOTE: May very well be overkill, node_traits
template <class TNode> struct node_traits;

template <>
struct node_traits<SinglyLinkedNode>
{
    static SinglyLinkedNode* null_node() { return nullptr; }
};

template <>
struct node_traits<DoublyLinkedNode>
{
    static DoublyLinkedNode* null_node() { return nullptr; }
};


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

template <>
struct node_next_extractor<DoublyLinkedNode>
{
    static inline DoublyLinkedNode* getNext(DoublyLinkedNode* current) { return current->getNext(); }
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



    /*
    operator value_type* () const
    {
        return TNodeAllocator::get_associated_value(base_t::getCurrent(), hint);
    } */

    value_type& operator*()
    {
        return *TNodeAllocator::get_associated_value(base_t::getCurrent(), hint);
    }

    bool operator==(const base_t& rhs)
    {
        return this->getCurrent() == rhs.getCurrent();
    }

    bool operator!=(const base_t& rhs)
    {
        return this->getCurrent() != rhs.getCurrent();
    }

    /*
    friend bool operator==(const base_t& lhs, const base_t& rhs)
    {
        return lhs.getCurrent() == rhs.getCurrent();
    } */

    /*
    friend bool operator!=(const base_t& lhs, const base_t& rhs)
    {
        return lhs.getCurrent() == rhs.getCurrent();
    } */



    /*
    friend bool operator==(const base_t& lhs, const base_t& rhs)
    {
        return *((value_type*)lhs) == *((value_type*)rhs);
    }

    friend bool operator!=(const base_t& lhs, const base_t& rhs)
    {
        return !(lhs == rhs);
    }*/
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
    bool empty() { return list.getHead(); }

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
    bool empty() { return list.getHead(); }

};

// FIX: naming = bad
// Manages allocator and basic iterators
template <class TNodeAllocator>
class test_list_base
{
protected:
    typedef typename TNodeAllocator::node_type   node_type;
    typedef typename TNodeAllocator::value_type  value_type;

    typedef value_type&                 reference;
    typedef const value_type&           const_reference;

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

    TNodeAllocator node_allocator;
};

#ifdef UNUSEDXXX
template <class TList, class TNode, class TNodeAllocator>
class list_base : public test_list_base<TNodeAllocator>
{
public:
    typedef TNode node_type;
    typedef TList list_type;

    typedef typename test_list_base<TNodeAllocator>::ForwardIterator         iterator;
    typedef const iterator   const_iterator;

protected:
    list_type list;

    TNodeAllocator& get_node_allocator()
    {
        return test_list_base<TNodeAllocator>::node_allocator;
    }

    node_type* _pop_front()
    {
        node_type* node = list.getHead();
        list.experimental_set_head(node->getNext());
        return node;
    }

public:
    bool empty() { return list.getHead(); }

    template <class TValue>
    // not a const like in standard because we expect to actually modify
    // the prev/next parts of value
    void push_front(TValue& value)
    {
        node_type* node = get_node_allocator().allocate(&value);

        list.insertAtBeginning(node);
    }


    void pop_front()
    {
        node_type* node = _pop_front();
        get_node_allocator().deallocate(node);
    }
};
#endif

// NOTE: Not sure what to do about std::initializer_list
// It's a "special" class which the compiler knows about and depends on.  Now sure if I
// can interact with it/make my own(that will get picked up) in an embedded environment
template <class T, class TNodeAllocator = node_allocator<T>>
class forward_list :
        public forward_list_base,
        public test_list_base<TNodeAllocator>
{
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;

    TNodeAllocator& get_node_allocator()
    {
        return test_list_base<TNodeAllocator>::node_allocator;
    }

public:
    typedef typename test_list_base<TNodeAllocator>::ForwardIterator         iterator;
    typedef const iterator   const_iterator;

    iterator begin() { return iterator(list.getHead()); }
    iterator end() { return iterator(nullptr); }

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
    iterator replace_after(const_iterator pos, value_type& value)
    {
        auto node_allocator = get_node_allocator();

        node_type* pos_node = pos.getCurrent();
        node_type* node_to_erase = pos_node->getNext();
        node_type* new_node = node_allocator.allocate(&value);

        // TODO: set pos_node->next to be &value
        new_node->insertBetween(pos_node, node_to_erase->getNext());

        node_allocator.deallocate(node_to_erase);
    }
};



template <class T, class TNodeAllocator = node_allocator<T, DoublyLinkedNode>>
class list :
        public list_base,
        public test_list_base<TNodeAllocator>
{
    typedef T                   value_type;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;

    TNodeAllocator& get_node_allocator()
    {
        return test_list_base<TNodeAllocator>::node_allocator;
    }

public:
    typedef typename test_list_base<TNodeAllocator>::ForwardIterator         iterator;
    typedef const iterator   const_iterator;

    iterator begin() { return iterator(list_base::list.getHead()); }
    iterator end() { return iterator(nullptr); }

    // not a const like in standard because we expect to actually modify
    // the prev/next parts of value
    void push_front(value_type& value)
    {
        node_type* node = get_node_allocator().allocate(&value);

        list_base::list.insertAtBeginning(node);
    }


    void pop_front()
    {
        // FIX: almost definitely this isn't gonna work right (not accounting for tail)
        node_type* node = list_base::pop_front();
        get_node_allocator().deallocate(node);
    }


    iterator insert_after(const_iterator pos, value_type& value)
    {
        node_type* pos_node = pos.getCurrent();
        node_type* node = get_node_allocator().allocate(&value);

        // FIX: insertBetween is overcompliated, the insert_after is cleaner and better
        // (the getNext() is always the value used, so why bother making it an explicit param)
        list_base::list.insertBetween(pos_node, pos_node->getNext(), node);

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
};

}

}}
