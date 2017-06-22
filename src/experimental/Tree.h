#pragma once

namespace FactUtilEmbedded { namespace experimental {

// experimentation with compile-time dependency tree
// didn't get very far

namespace layer2 {


template <class T>
class NodeBase
{
    const T value;

public:
    NodeBase(T value) : value(value) {}
};


template <class T, Node<T>& TParentNode>
class Node : public NodeBase<T>
{
    typedef NodeBase<T> node_t;

public:
    public Node(T value) : node_t(value) {}
};


class Tree
{

};

}

}}