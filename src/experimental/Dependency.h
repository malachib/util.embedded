#pragma once

//#define FEATURE_DEPENDENCY_EXPERIMENTAL_INIT

namespace FactUtilEmbedded { namespace experimental {

/**
 * Parameters:
 *
 * @param parent_id id of parent of the item being processed
 * @param id id of the actual item being processed
 */
typedef void (*responder_t)(int parent_id, int id);

template <int ...vectors>
class Vector
{
    // bool is just dummy value for now since my variadic
    // is proficient but not expert
    // otherwise we get an ambiguity error
    template <bool>
    static bool _contains(int value)
    {
        return false;
    }

    template <bool, int v, int ..._vectors>
    static bool _contains(int value)
    {
        if(v == value)
            return true;

        return _contains<false, _vectors...>(value);
    }

public:
    static bool contains(int value)
    {
        return _contains<false, vectors...>(value);
    }

    static constexpr int size()
    {
        return sizeof...(vectors);
    }

    template <int value>
    static auto add() -> decltype(Vector<value, vectors...>())
    {
    }

    //static Vector<3, vectors> add_() {}
};

template <class T>
class Dependent
{
public:
    template <class ...TArgs>
    class On
    {
    public:
        typedef T t_t;

        template <responder_t responder, class TAlreadyVisited, class TDM, int parent_id>
        static void _walk_across()
        {

        }

        template <responder_t responder, class TAlreadyVisited, class TDM, int parent_id, class T2, class ...TArgs2>
        static void _walk_across()
        {
            _walk_across<responder, TAlreadyVisited, TDM, parent_id, TArgs2...>();

            // Now, here we need to dig into the On:: for each child represented by T2,
            // so that we can recursively walk down in and walk across from there (if needed)
            // so perhaps we need to retrieve that from the manager via the ID
            //TDM::get<parent_id>();
            TDM::template walk2<T2::ID, responder, TAlreadyVisited>();

            //if(!TAlreadyVisited::contains(T2::ID))
                responder(parent_id, T2::ID);
        }

        // walks across children only, not parent
        template <responder_t responder, class TAlreadyVisited, class TDM, int parent_id>
        static void walk_across()
        {
            _walk_across<responder, TAlreadyVisited, TDM, parent_id, TArgs...>();
        }


        template <class TDM, class TContext>
        static void _walk_across2()
        {

        }

        template <class TDM, class TContext, class T2, class ...TArgs2>
        static void _walk_across2()
        {
            TContext::template top_callback<t_t, T2>();

            _walk_across2<TDM, TContext, TArgs2...>();

            // Now, here we need to dig into the On:: for each child represented by T2,
            // so that we can recursively walk down in and walk across from there (if needed)
            // so perhaps we need to retrieve that from the manager via the ID
            TDM::template walk5<T2::ID, TContext>();

            TContext::template callback<t_t, T2>();
        }


        template <class TDM, class TContext>
        static void walk_across2()
        {
            _walk_across2<TDM, TContext, TArgs...>();
        }
    };

};


template <class ...TArgs>
class DependencyManager
{
    template <int id>
    static void _get()
    {

    };

    template <int id, class T, class ...TArgs2>
    static T _get()
    {
        if(id == T::t_t::ID)
            //return *((T *) nullptr);
            return std::declval<T>;

        return _get<id, TArgs2...>();
    }

    // walk over a specific ID only
    template <int id, responder_t responder, class TAlreadyVisited>
    static void _walk2()
    {

    }

    // walk over a specific ID only
    template <int id, responder_t responder, class TAlreadyVisited, class T, class ...TArgs2>
    static void _walk2()
    {
        typedef typename T::t_t t_t;
        constexpr int ID = t_t::ID;

        if(ID == id)
        {
            //responder(0, id);
            auto size = TAlreadyVisited::size();
            typedef decltype(TAlreadyVisited::template add<ID>()) already_visited_t;
            size = already_visited_t::size();
            T::template walk_across<responder, already_visited_t, DependencyManager<TArgs...>, ID>();
            return;
            //_walk2<id, responder, already_visited_t, TArgs2...>();
        }

        _walk2<id, responder, TAlreadyVisited, TArgs2...>();
    }

    template <int id, class TContext, int parent_id>
    static void _walk5()
    {

    }

    template <int id, class TContext, int parent_id, class T2, class ...TArgs2>
    static void _walk5()
    {
        typedef typename T2::t_t t_t;
        constexpr int ID = t_t::ID;

        if(id == ID)
        {
            if(parent_id == id)
                TContext::template top_callback<t_t, t_t>();

            T2::template walk_across2<DependencyManager<TArgs...>, TContext>();
            if(parent_id == id)
                TContext::template callback<t_t, t_t>();
            return;
        }

        _walk5<id, TContext, parent_id, TArgs2...>();
    }


public:

    // Get the 'On' class for this particular ID
    template <const int id>
    static auto get() -> decltype(_get<id, TArgs...>())
    {
        //return _get<id, TArgs...>();
    }

    // walk over everything under a specific ID
    template <int id, responder_t responder, class TAlreadyVisited>
    static void walk2()
    {
        //typedef TAlreadyVisited already_visited_t;
        typedef Vector<> already_visited_t;

        // FIX: Have a serious issue, passing in TAlreadyVisited like we want to causes the compiler to go nuts
        // probably it can't resolve the cross-recursion between DependencyManager and Dependent::On
        _walk2<id, responder, already_visited_t, TArgs...>();
    }

    template <int id, responder_t responder>
    static void walk3()
    {
        // TESTING ONLY
        typedef Vector<> already_visited_t;

        walk2<id, responder, already_visited_t>();
        responder(0, id);
    }

    template <int id, class TClassCallback>
    static void walk4()
    {

    };

    template <int id, class TContext, int parent_id = -1>
    static void walk5()
    {
        _walk5<id, TContext, parent_id, TArgs...>();
    }
};

}}