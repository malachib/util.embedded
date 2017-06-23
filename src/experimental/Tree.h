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


template <class ...TNodes>
class Tree
{
    template <bool dummy>
    static uint16_t _get_parent(uint16_t id)
    {
        return -1;
    }

    template <bool dummy, class TNode, class ...TNodes2>
    static uint16_t _get_parent(uint16_t id)
    {
        if(TNode::_id == id) return TNode::_parent_id;

        return _get_parent<true, TNodes2...>(id);
    }

    template <bool dummy>
    static uint16_t _get_child(uint16_t id, uint16_t index)
    {
        return -1;
    }

    template <bool dummy, class TNode, class ...TNodes2>
    static uint16_t _get_child(uint16_t id, uint16_t index)
    {
        if(TNode::_parent_id == id)
        {
            // Decrement index counter to "move up" the indexes
            // to grab specific child
            if(index-- == 0) return TNode::_id;
        }

        return _get_child<true, TNodes2...>(id, index);
    }


    template <bool dummy>
    static uint16_t _child_count(uint16_t id)
    {
        return 0;
    }

    template <bool dummy, class TNode, class ...TNodes2>
    static uint16_t _child_count(uint16_t id)
    {
        if(TNode::_parent_id == id)
        {
            return _child_count<true, TNodes2...>(id) + 1;
        }
        else
            return _child_count<true, TNodes2...>(id);
    }

    inline static void dummy(uint16_t, uint16_t) {}

public:
    static inline uint16_t get_parent(uint16_t id)
    {
        return _get_parent<true, TNodes...>(id);
    }


    static inline uint16_t get_child(uint16_t id, uint16_t index)
    {
        return _get_child<true, TNodes...>(id, index);
    }


    static inline size_t child_count(uint16_t id)
    {
        return _child_count<true, TNodes...>(id);
    }

    typedef void (*fn_responder_t)(uint16_t id, uint16_t parent_id) ;

    //template <fn_responder_t responder, fn_responder_t responder_up = nullptr, bool top = true>
    template <class  TResponderFunc, class TResponderFuncUp, bool top = true>
    static inline void walk(uint16_t start_id, TResponderFunc responder, TResponderFuncUp responder_up)
    {
        if(top)
        //if(top && responder)
            responder(start_id, get_parent(start_id));

        uint16_t count = child_count(start_id);

        for(uint16_t i = 0; i < count; i++)
        {
            uint16_t child_id = get_child(start_id, i);
            //if(responder)
                responder(child_id, start_id);
            walk<TResponderFunc, TResponderFuncUp, false>(child_id, responder, responder_up);
            if(responder_up) responder_up(child_id, start_id);
        }

        if(top && responder_up) responder_up(start_id, get_parent(start_id));
    }

    //template <fn_responder_t responder, fn_responder_t responder_up = nullptr, bool top = true>
    template <class  TResponderFunc>
    static inline void walk(uint16_t start_id, TResponderFunc responder)
    {
        walk(start_id, responder, dummy);
    };

    template <class  TResponderFunc>
    static inline void walk_up(uint16_t start_id, TResponderFunc responder)
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