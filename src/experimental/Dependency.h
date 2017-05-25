#pragma once

namespace FactUtilEmbedded { namespace experimental {

template <class T>
class Dependent
{
public:
    template <class ...TArgs>
    class On
    {

    };

};


template <class ...TArgs>
class DependencyManager
{

};

}}