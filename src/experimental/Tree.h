#pragma once

namespace FactUtilEmbedded { namespace experimental {

// experimentation with compile-time dependency tree
// didn't get very far

namespace layer1 {

template <uint16_t id>
class NodeBase
{
};


template <uint16_t id, uint16_t parent_id>
class Node : public NodeBase<id>
{
    typedef NodeBase<id> node_base_t;
    typedef Node<id, parent_id> node_t;

public:

    static constexpr uint16_t _id = id;
    static constexpr uint16_t _parent_id = parent_id;

};


template <class T, T id, T parent_id>
class GenericNode
{
public:
    static constexpr T _id = id;
    static constexpr T _parent_id = parent_id;
};


template <void* id, void* parent_id>
class GenericNode2
{
public:
    static constexpr void* _id = id;
    static constexpr void* _parent_id = parent_id;
};

template <class TKey>
struct TreeKeyTraits
{
    static TKey null_node() { return nullptr; }
};

template <>
struct TreeKeyTraits<uint16_t>
{
    static uint16_t null_node() { return -1; }
};

/*
template <>
struct TreeKeyTraits<void*>
{
    static void* null_node() { return nullptr; }
}; */

template <class TKey, class ...TNodes>
class Tree
{
    typedef TKey key_t;
    typedef TreeKeyTraits<TKey> key_traits_t;

    template <bool dummy>
    static constexpr key_t _get_parent(key_t id)
    {
        return key_traits_t::null_node();
    }

    // TODO: maybe change all this to return TNode itself
    // so that we can then resolve out _parent_id actual type
    template <bool dummy, class TNode, class ...TNodes2>
    static constexpr key_t _get_parent(key_t id)
    {
        // Making it one line allows return value to be constexpr
        return TNode::_id == id ? TNode::_parent_id : _get_parent<true, TNodes2...>(id);
        /*
        if(TNode::_id == id) return TNode::_parent_id;

        return _get_parent<true, TNodes2...>(id); */
    }

    template <bool dummy>
    static constexpr key_t _get_child(key_t id, key_t index)
    {
        return key_traits_t::null_node();
    }

    template <bool dummy, class TNode, class ...TNodes2>
    static constexpr key_t _get_child(key_t id, key_t index)
    {
        return (TNode::_parent_id == id && index-- == 0) ?
               TNode::_id :
               _get_child<true, TNodes2...>(id, index);
        /*
        if(TNode::_parent_id == id)
        {
            // Decrement index counter to "move up" the indexes
            // to grab specific child
            if(index-- == 0) return TNode::_id;
        }

        return _get_child<true, TNodes2...>(id, index); */
    }


    template <bool dummy>
    static constexpr size_t _child_count(key_t id)
    {
        return 0;
    }

    template <bool dummy, class TNode, class ...TNodes2>
    static constexpr size_t _child_count(key_t id)
    {
        // One return-line permits constexpr
        return _child_count<true, TNodes2...>(id) + (TNode::_parent_id == id ? 1 : 0);
    }

    inline static void dummy(key_t, key_t) {}

public:
    // TODO: change so that we return decltype of found node parent id type
    static inline constexpr key_t get_parent(key_t id)
    {
        return _get_parent<true, TNodes...>(id);
    }


    // TODO: change so that we return decltype of found node child id type
    static inline constexpr key_t get_child(key_t id, key_t index)
    {
        return _get_child<true, TNodes...>(id, index);
    }


    static inline constexpr size_t child_count(key_t id)
    {
        return _child_count<true, TNodes...>(id);
    }

    typedef void (*fn_responder_t)(key_t id, key_t parent_id) ;

    // might need a 2nd version of this which takes a templated start_id to really get it to
    // flatten out.  Not sure, need to look at disassemblies
    //template <fn_responder_t responder, fn_responder_t responder_up = nullptr, bool top = true>
    template <class  TResponderFunc, class TResponderFuncUp, bool top = true>
    static constexpr inline void walk(const key_t start_id, TResponderFunc responder, TResponderFuncUp responder_up)
    {
        // using autos in here so that constexpr gets a fair share
        // NOT sure if it really matters

        if(top)
        //if(top && responder)
            responder(start_id, get_parent(start_id));

        size_t count = child_count(start_id);

        for(uint16_t i = 0; i < count; i++)
        {
            const key_t child_id = get_child(start_id, i);
            //if(responder)
                responder(child_id, start_id);
            walk<TResponderFunc, TResponderFuncUp, false>(child_id, responder, responder_up);
            if(responder_up) responder_up(child_id, start_id);
        }

        if(top && responder_up) responder_up(start_id, get_parent(start_id));
    }

    //template <fn_responder_t responder, fn_responder_t responder_up = nullptr, bool top = true>
    template <class  TResponderFunc>
    static inline constexpr void walk(key_t start_id, TResponderFunc responder)
    {
        walk(start_id, responder, dummy);
    };

    template <class  TResponderFunc>
    static inline constexpr void walk_up(key_t start_id, TResponderFunc responder)
    {
        walk(start_id, dummy, responder);
    };
};



}

namespace layer2 {


template <uint16_t id, class T>
class NodeBase
{
    const T value;

public:
    NodeBase(T value) : value(value) {}
};


template <uint16_t id, uint16_t parent_id, class T>
class Node : public NodeBase<id, T>
{
    typedef NodeBase<id, T> node_base_t;
    typedef Node<id, parent_id, T> node_t;

public:
    Node(T value) : node_base_t(value) {}
};


template <class ...TNodes>
class Tree
{
public:
    Tree(TNodes...nodes)
    {
    }

};

}

}}