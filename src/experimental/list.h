#pragma once

namespace FactUtilEmbedded { namespace std {

namespace experimental {

template<class T>
class list
{

};

struct forward_iterator_tag {};

template <class T>
class forward_list
{
    typedef forward_iterator_tag iterator;
    typedef T value_type;

public:
};

}

}}